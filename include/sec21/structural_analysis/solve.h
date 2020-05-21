#pragma once

#include <sec21/units.h>
#include <sec21/zip.h>
#include <sec21/erase_if_index.h>
#include <sec21/for_each_chunk.h>
#include <sec21/numeric/make_vector.h>
#include <sec21/units/quantity.h>
#include <sec21/structural_analysis/system_result.h>
#include <sec21/structural_analysis/impl/lookup.h>
//! \todo remove/replace header
#include <sec21/structural_analysis/impl/matrix_helper.h>
#include <sec21/structural_analysis/impl/geometry.h>
#include <sec21/structural_analysis/impl/global_stiffness_matrix.h>
#include <sec21/structural_analysis/impl/load_vector.h>

#include <viennacl/vector.hpp>
#include <viennacl/matrix.hpp>
#include <viennacl/linalg/lu.hpp>

#include <vector>
#include <map>
#include <string_view>

//! \todo remove
template <typename Iterator>
auto dump(Iterator first, Iterator last, std::string_view name)
{
   std::cout << name.data() << "n: "<< std::distance(first, last) << "\n(";
   std::copy(first, last, std::ostream_iterator<typename std::iterator_traits<Iterator>::value_type>(std::cout, ", "));
   std::cout << ")\n" << std::endl;
}
template <typename Sequence>
auto dump(Sequence&& seq, std::string_view name)
{
   dump(std::begin(seq), std::end(seq), std::move(name));
}

namespace sec21::structural_analysis
{
   // template <typename System>
   // struct system_result;

   namespace impl
   {
      //! \todo name
      template <typename System>
      [[nodiscard]] auto solve_with_load(System const &sys, boost::numeric::ublas::vector<typename System::precision_t> F)
      {
         auto lookup = impl::make_lookup(sys, impl::Row{0});

         std::vector<bool> mask{};
         support_mask(std::begin(sys.nodes), std::end(sys.nodes), std::back_inserter(mask));

         decltype(lookup) supported_rows{};
         decltype(lookup) not_supported_rows{};

         partition_lookup(
            std::begin(lookup), 
            std::end(lookup),
            std::begin(mask), 
            std::end(mask),
            std::back_inserter(supported_rows),
            std::back_inserter(not_supported_rows));

         const auto supported_cols = impl::row_to_col(std::begin(supported_rows), std::end(supported_rows));

         auto K = impl::global_stiffness_matrix(sys);
         auto K_without_supports = impl::remove_from_matrix(K, supported_rows, supported_cols);
         auto K_strich = impl::remove_from_matrix(K, not_supported_rows, supported_cols);

std::cout << "K: " << K << std::endl;
std::cout << "K_without_supports: " << K_without_supports << std::endl;
std::cout << "K_strich: " << K_strich << std::endl;

         system_result<System> result;

         using precision_t = typename System::precision_t;

         using namespace boost::numeric;
         using namespace viennacl::linalg;

         viennacl::vector<precision_t> vcl_rhs(F.size());
         //! \todo names
         viennacl::vector<precision_t> vcl_result_gl1(K_without_supports.size2());
         viennacl::matrix<precision_t> vcl_matrix(K_without_supports.size1(), K_without_supports.size2());

         viennacl::copy(F, vcl_rhs);
         viennacl::copy(K_without_supports, vcl_matrix);

         vcl_result_gl1 = solve(vcl_matrix, vcl_rhs, viennacl::linalg::upper_tag{});
         vcl_result_gl1 = solve(vcl_matrix, vcl_rhs, viennacl::linalg::lower_tag{});
         lu_factorize(vcl_matrix);
         lu_substitute(vcl_matrix, vcl_rhs);

         std::vector<precision_t> verschiebungen(K_without_supports.size1());
         viennacl::copy(std::begin(vcl_rhs), std::end(vcl_rhs), std::begin(verschiebungen));

         ublas::vector<precision_t> auflagerreaktionen(K_strich.size1());

         viennacl::vector<precision_t> vcl_u(std::size(verschiebungen));
         viennacl::vector<precision_t> vcl_result(std::size(auflagerreaktionen));
         viennacl::matrix<precision_t> vcl_A(K_strich.size1(), K_strich.size2());

         viennacl::copy(verschiebungen, vcl_u);
         viennacl::copy(K_strich, vcl_A);

         vcl_result = viennacl::linalg::prod(vcl_A, vcl_u);
         viennacl::copy(std::begin(vcl_result), std::end(vcl_result), std::begin(auflagerreaktionen));

         const auto n = std::distance(std::begin(sys.nodes), std::end(sys.nodes));
         constexpr auto dim = System::dimension_v;

         //! \todo: possible test: expects(std::size(lookup) == n * dim);
         std::vector<precision_t> support_reactions(n * dim);
         std::vector<precision_t> displacements(n * dim);
         
         dump(support_reactions, "support_reactions");
         dump(displacements, "displacements");

         //! \todo could be only on zip() call -> benchmark
         zip(
            [&support_reactions](auto&& tuple) { support_reactions[static_cast<int>(std::get<impl::Row>(tuple))] = std::get<1>(tuple); }, 
            std::begin(supported_rows), 
            std::end(supported_rows), 
            std::begin(auflagerreaktionen));
         zip(
            [&displacements](auto&& tuple) { displacements[static_cast<int>(std::get<impl::Row>(tuple))] = std::get<1>(tuple); }, 
            std::begin(not_supported_rows), 
            std::end(not_supported_rows), 
            std::begin(verschiebungen));

         // allocate space for the node results
         // std::transform(        
         //    std::begin(sys.nodes), 
         //    std::end(sys.nodes),
         //    std::inserter(result.nodes, std::end(result.nodes)),
         //    [](auto&& node){ return { node.id, {}, {} }; });

         // std::transform(        
         //    std::begin(sys.members), 
         //    std::end(sys.members),
         //    std::inserter(result.members, std::end(result.members)),
         //    [&result](auto&& member){ return typename decltype(result)::node_result_t{ }; });

         std::transform(        
            std::begin(sys.nodes), 
            std::end(sys.nodes),
            std::inserter(result.node, std::end(result.node)),
            //! \todo find a better solution for "typename decltype()"
            [&result](auto&& node){ return std::make_pair(node.id, typename decltype(result)::node_result{}); });

         for_each_chunk<dim>(
            std::begin(support_reactions), 
            std::end(support_reactions), 
            [it = std::begin(result.node)](auto x, auto y) mutable 
            {
               using value_t = typename decltype(it->second.support_reaction)::value_type;
               it->second.support_reaction = { value_t{ x }, value_t{ y } };
               it++;
            });

         for_each_chunk<dim>(
            std::begin(displacements), 
            std::end(displacements), 
            [it = std::begin(result.node)](auto x, auto y) mutable 
            {
               using value_t = typename decltype(it->second.displacement)::value_type;
               it->second.displacement = { value_t{ x }, value_t{ y } };
               it++;
            });

         //! \todo way to complex and not parallelizable
         for (auto const& m : sys.members)
         {
            const auto[s, e] = sys.coincidence_table.at(m.id);

            auto delta_s = result.node[s].displacement;
            auto delta_e = result.node[e].displacement;

            const auto EA = m.E * m.A;
            const auto EA_over_l = EA / length(sys, m.id).value();         

            const auto alpha = impl::angle_to_x_axis(sys, m.id);
            //! \todo k1?
            const auto k1 = alpha < 0.0 ? -1.0 : 1.0;
            //! \todo k2?
            const auto k2 = 1;

            const auto N = (
               -std::cos(alpha) * X(delta_s).value() +
               -std::sin(alpha) * Y(delta_s).value() +
               std::cos(alpha) * X(delta_e).value() +
               std::sin(alpha) * Y(delta_e).value()) * k1 * EA_over_l * k2;

            //! \todo: ungenauigkeit in der berechung
            result.member[m.id].normal_force = N;

            result.members.insert({ m.id, N });
         }

         for (const auto [node, res] : result.node)
            result.nodes.insert({ node, res.displacement, res.support_reaction });

         return std::tuple{true, result};
      }
   }

   template <typename System>
   [[nodiscard]] auto solve(System const &sys)
   {
      //! \todo lookup table is twice required -> consider it as argument
      auto lookup = impl::make_lookup(sys, impl::Row{0});

      std::vector<bool> mask{};
      support_mask(std::begin(sys.nodes), std::end(sys.nodes), std::back_inserter(mask));

      decltype(lookup) supported_rows{};
      decltype(lookup) not_supported_rows{};

      partition_lookup(
         std::begin(lookup), 
         std::end(lookup),
         std::begin(mask), 
         std::end(mask),
         std::back_inserter(supported_rows),
         std::back_inserter(not_supported_rows));

      const auto load_of_all_nodes = impl::load_vector(sys);
      const auto load_without_supports = erase_if_index(
         load_of_all_nodes, 
         [&](auto i){ return contains(supported_rows, impl::Row{i}); });

      // convert quantities to a more suitable datatype, in this case to ublas::vector<double>
      auto F = numeric::make_vector(
         std::begin(load_without_supports), 
         std::end(load_without_supports), 
         [](auto&& q) -> double { return q.value(); });

      return impl::solve_with_load(sys, F);
   }
}
