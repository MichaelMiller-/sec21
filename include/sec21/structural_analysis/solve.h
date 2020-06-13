#pragma once

#include <sec21/units.h>
#include <sec21/zip.h>
#include <sec21/erase_if_index.h>
#include <sec21/for_each_chunk.h>
#include <sec21/numeric/make_vector.h>
#include <sec21/units/quantity.h>
#include <sec21/structural_analysis/common.h>
#include <sec21/structural_analysis/system_result.h>
#include <sec21/structural_analysis/impl/lookup.h>
#include <sec21/structural_analysis/impl/matrix_helper.h>
#include <sec21/structural_analysis/impl/geometry.h>
#include <sec21/structural_analysis/impl/global_stiffness_matrix.h>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/io.hpp>

// #pragma GCC diagnostic push
// #pragma GCC diagnostic ignored "-Wignored-attributes"
   #include <viennacl/vector.hpp>
   #include <viennacl/matrix.hpp>
   #include <viennacl/linalg/lu.hpp>
// #pragma GCC diagnostic pop

#include <vector>
#include <map>
#include <string_view>

namespace sec21::structural_analysis
{
   namespace impl
   {
      template <typename System>
      [[nodiscard]] auto solve(
         System const &sys, 
         boost::numeric::ublas::matrix<typename System::precision_t> K,
         boost::numeric::ublas::vector<typename System::precision_t> F)
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

         const auto K_without_supports = impl::remove_from_matrix(K, supported_rows, supported_cols);
         const auto K_strich = impl::remove_from_matrix(K, not_supported_rows, supported_cols);
         const auto F_without_supports = impl::remove_from_vector(F, supported_rows);

         // consider sanitiy checks:
         // F.size1() == K_without_supports.size1() == K_without_supports.size2()
         // F.size1() == K_strich.size2()
         // expects(std::size(lookup) == n * dim);

         using precision_t = typename System::precision_t;

         using namespace boost::numeric;
         using namespace viennacl::linalg;

         viennacl::vector<precision_t> vcl_rhs(F_without_supports.size());
         //! \todo names
         viennacl::vector<precision_t> vcl_result_gl1(K_without_supports.size2());
         viennacl::matrix<precision_t> vcl_matrix(K_without_supports.size1(), K_without_supports.size2());

         viennacl::copy(F_without_supports, vcl_rhs);
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

         std::vector<precision_t> support_reactions(n * dim);
         std::vector<precision_t> displacements(n * dim);
         
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

         system_result<System> result;

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
            [&result](auto&& node){ return std::make_pair(node.id, typename decltype(result)::node_result{}); });

         auto it = std::begin(result.node);
         for_each_chunk<dim>(
            std::begin(support_reactions), 
            std::end(support_reactions), 
            //! \todo is not working with msvc: [it = std::begin(result.node)](auto x, auto y) mutable 
            [&it](auto x, auto y)
            {
               using value_t = typename decltype(it->second.support_reaction)::value_type;
               it->second.support_reaction = { value_t{ x }, value_t{ y } };
               it++;
            });

         it = std::begin(result.node);
         for_each_chunk<dim>(
            std::begin(displacements), 
            std::end(displacements), 
            //! \todo is not working with msvc: [it = std::begin(result.node)](auto x, auto y) mutable 
            [&it](auto x, auto y)
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

            const auto EA = m.E.value() * m.A.value();
            const auto kv = EA / length(sys, m.id).value();
            // const auto kv = EA_over_l(m.E, m.A, length(sys, m.id)).value();         

            const auto alpha = impl::angle_to_x_axis(sys, m.id);
            //! \todo k1?
            const auto k1 = alpha < 0.0 ? -1.0 : 1.0;

            const auto N = (
               -std::cos(alpha) * X(delta_s).value() +
               -std::sin(alpha) * Y(delta_s).value() +
                std::cos(alpha) * X(delta_e).value() +
                std::sin(alpha) * Y(delta_e).value()) * k1 * kv;

            //! \todo: ungenauigkeit in der berechung
            result.member[m.id].normal_force = N;

            result.members.insert({ m.id, N });
         }

         for (const auto [node, res] : result.node)
            result.nodes.insert({ node, res.displacement, res.support_reaction });

         return std::tuple{ true, result };
      }
   }

   template <typename System>
   [[nodiscard]] auto solve(System const& sys, loadcase<System> const& load) // -> std::tuple<bool, system_result<typename System>>
   {
      using value_t = units::quantity<units::newton, typename System::precision_t>;

      constexpr auto dim = System::dimension_v;

      std::vector<value_t> action_on_structure(std::size(sys.nodes) * dim);

      add_node_load(sys, load, action_on_structure);
      add_temperature_load(sys, load, action_on_structure);

      //! \todo parallelize K and F
      auto K = impl::global_stiffness_matrix(sys);
      // convert quantities to a more suitable datatype, in this case to ublas::vector<T>
      auto F = numeric::make_vector(
         std::begin(action_on_structure), 
         std::end(action_on_structure), 
         [](auto&& q) -> typename System::precision_t { return q.value(); });

      return impl::solve(sys, K, F);

      //! \todo process(solve) -> post_process(sys, result)
   }   
}
