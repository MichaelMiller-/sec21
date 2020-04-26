#pragma once

#include <sec21/units.h>
#include <sec21/zip.h>
#include <sec21/erase_if_index.h>
#include <sec21/for_each_chunk.h>
#include <sec21/numeric/make_vector.h>
#include <sec21/units/quantity.h>
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

namespace sec21::structural_analysis
{
   namespace impl
   {
      //! \todo find a better solution
      constexpr auto is_node_in_dir_supported = [](auto&& node, auto&& dir) noexcept
      {
         if (node.support == Support::Roller && dir == Axis::Y)
            return true;
         if (node.support == Support::Hinge)
            return true;
         return false;
      };
   }
      
   template <typename System>
   struct system_result
   {
      static constexpr auto dimension_v = System::dimension_v;

      using precision_t = typename System::precision_t;
      using node_t = typename System::node_t;
      using member_t = typename System::member_t;
      using node_descriptor_t = typename node_t::descriptor_t;
      using member_descriptor_t = typename member_t::descriptor_t;

      using displacement_t = units::quantity<units::millimeter, precision_t>;
      using force_t = units::quantity<units::newton, precision_t>;

      struct node_result
      {
         std::array<displacement_t, dimension_v> displacement{};
         std::array<force_t, dimension_v> support_reaction{};
         //! \todo or boost::qvm
         // boost::qvm::vec<displacement_t, dimension_v> displacement{};
      };
      struct member_result
      {
         force_t normal_force{};
      };

      std::map<node_descriptor_t, node_result> node;
      std::map<member_descriptor_t, member_result> member;
   };

   template <typename System>
   [[nodiscard]] auto solve(System const &sys)
   {
      constexpr auto dim = System::dimension_v;

      const auto n = std::distance(std::begin(sys.nodes), std::end(sys.nodes));
      const auto lookup = impl::make_lookup(sys, impl::Row{0});
      const auto festgehaltende_zeilen = impl::filter(sys, lookup, impl::is_node_in_dir_supported);
      const auto festgehaltende_spalten = impl::row_to_col(std::begin(festgehaltende_zeilen), std::end(festgehaltende_zeilen));

      //! \todo 
      constexpr auto is_not = [&](auto&& node, auto&& dir) {
         return impl::is_node_in_dir_supported(node, dir) == false;
      };
      const auto verschiebungs_zeilen = impl::filter(sys, lookup, is_not);

      auto K = impl::global_stiffness_matrix(sys);
      auto K_without_supports = impl::remove_from_matrix(K, festgehaltende_zeilen, festgehaltende_spalten);

      system_result<System> result;

      using precision_t = typename System::precision_t;

      using namespace boost::numeric;
      using namespace viennacl::linalg;

      auto load_of_all_nodes = impl::load_vector(sys);
      auto load_without_supports = erase_if_index(
         load_of_all_nodes, 
         [&](auto i){ return contains(festgehaltende_zeilen, impl::Row{i}); });
      // convert quantities to a simple datatype, in this case to double
      auto F = numeric::make_vector(
         std::begin(load_without_supports), 
         std::end(load_without_supports), 
         [](auto&& q) -> double { return q.value(); });

      viennacl::vector<precision_t> vcl_rhs(F.size());
      //! \todo names
      viennacl::vector<precision_t> vcl_result_gl1(K_without_supports.size2());
      viennacl::matrix<precision_t> vcl_matrix(K_without_supports.size1(), K_without_supports.size2());

      viennacl::copy(F, vcl_rhs);
      viennacl::copy(K_without_supports, vcl_matrix);

      vcl_result_gl1 = solve(vcl_matrix, vcl_rhs, upper_tag{});
      vcl_result_gl1 = solve(vcl_matrix, vcl_rhs, lower_tag{});
      lu_factorize(vcl_matrix);
      lu_substitute(vcl_matrix, vcl_rhs);

      std::vector<precision_t> verschiebungen(K_without_supports.size1());
      viennacl::copy(std::begin(vcl_rhs), std::end(vcl_rhs), std::begin(verschiebungen));

      auto K_strich = impl::remove_from_matrix(K, verschiebungs_zeilen, festgehaltende_spalten);

      ublas::vector<precision_t> auflagerreaktionen(K_strich.size1());

      viennacl::vector<precision_t> vcl_u(std::size(verschiebungen));
      viennacl::vector<precision_t> vcl_result(std::size(auflagerreaktionen));
      viennacl::matrix<precision_t> vcl_A(K_strich.size1(), K_strich.size2());

      viennacl::copy(verschiebungen, vcl_u);
      viennacl::copy(K_strich, vcl_A);

      vcl_result = viennacl::linalg::prod(vcl_A, vcl_u);
      viennacl::copy(std::begin(vcl_result), std::end(vcl_result), std::begin(auflagerreaktionen));

      //! \todo: possible test: expects(std::size(lookup) == n * dim);
      std::vector<precision_t> support_reactions(n * dim);
      std::vector<precision_t> displacements(n * dim);

      //! \todo could be only on zip() call -> benchmark
      zip(
         [&support_reactions](auto&& tuple) { support_reactions[static_cast<int>(std::get<impl::Row>(tuple))] = std::get<1>(tuple); }, 
         std::begin(festgehaltende_zeilen), 
         std::end(festgehaltende_zeilen), 
         std::begin(auflagerreaktionen));
      zip(
         [&displacements](auto&& tuple) { displacements[static_cast<int>(std::get<impl::Row>(tuple))] = std::get<1>(tuple); }, 
         std::begin(verschiebungs_zeilen), 
         std::end(verschiebungs_zeilen), 
         std::begin(verschiebungen));

      // allocate space for the node results
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
            //! \todo what is this factor k?
            static constexpr auto k = 0.000036;
            using value_t = typename decltype(it->second.displacement)::value_type;
            it->second.displacement = { value_t{ x * k }, value_t{ y * k } };
            it++;
         });

      //! \todo way to complex and not parallelizable
      for (auto&& m : sys.members)
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
         const auto k2 = 0.1;

         const auto N = (
            -std::cos(alpha) * X(delta_s).value() +
            -std::sin(alpha) * Y(delta_s).value() +
            std::cos(alpha) * X(delta_e).value() +
            std::sin(alpha) * Y(delta_e).value()) * k1 * EA_over_l * k2;

         //! \todo: ungenauigkeit in der berechung
         result.member[m.id].normal_force = N;
      }

      return std::tuple{true, result};
   }
}