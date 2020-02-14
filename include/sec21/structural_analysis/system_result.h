#pragma once

#include <sec21/units.h>
#include <sec21/log.h>
#include <sec21/zip.h>

#include <sec21/structural_analysis/impl/lookup.h>
#include <sec21/structural_analysis/impl/matrix_helper.h>
#include <sec21/structural_analysis/impl/zuordnungsmatrix.h>
#include <sec21/structural_analysis/impl/geometry.h>
#include <sec21/structural_analysis/impl/global_stiffness_matrix.h>
#include <sec21/structural_analysis/impl/normalkraft.h>
#include <sec21/structural_analysis/impl/lastvektor_f.h>

#include <viennacl/vector.hpp>
#include <viennacl/matrix.hpp>
#include <viennacl/linalg/lu.hpp>

#include <vector>

namespace sec21::structural_analysis
{
   namespace impl
   {
      //! \tod find a better solution
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
      // struct node_result
      // {
      //    using node_t = typename System::node_t;
      // };

      // struct member_result
      // {
      //    using member_t = typename System::member_t;
      // };

      using precision_t = typename System::precision_t;
      //    using node_t = typename System::node_t;
      // using node_descriptor_t = typename node_t::descriptor_t;
      // using force_t = node_t::force_t;
      // using member_descriptor_t = member_t::descriptor_t;

      //! \todo type seams odd
      // using verschiebung_t = std::array<unit::millimeter<precision_t>, System::dimension_v>;

      std::vector<double> verschiebungen{};
      // Schnittgroessen
      std::vector<double> auflagerreaktionen{};
      std::vector<double> normalkraefte{};
   };

   template <typename System, typename Log = log>
// #ifdef __cpp_concepts
//    requires SpaceTruss<System>
// #endif
   [[nodiscard]] auto solve(System const &sys)
   {
      constexpr auto dim = System::dimension_v;

      const auto n = std::distance(std::begin(sys.nodes), std::end(sys.nodes));
      //! \todo move lookup to impl
      const auto lookup = impl::make_lookup(sys, impl::Row{0});
      const auto festgehaltende_zeilen = impl::filter(sys, lookup, impl::is_node_in_dir_supported);
      const auto festgehaltende_spalten = impl::row_to_col(std::begin(festgehaltende_zeilen), std::end(festgehaltende_zeilen));
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

      std::vector<precision_t> verschiebungen(K_without_supports.size1());

      //! \todo to complex!
      auto gg = impl::lastvektor_F(sys);
      decltype(gg) ff;
      for (auto i = 0; i < std::size(gg); ++i)
      {
         if (contains(festgehaltende_zeilen, impl::Row{i}))
            continue;
         ff.push_back(gg[i]);
      }
      auto F = impl::make_vector(std::begin(ff), std::end(ff));

      viennacl::vector<precision_t> vcl_rhs(F.size());
      viennacl::vector<precision_t> vcl_result_gl1(K_without_supports.size2());
      viennacl::matrix<precision_t> vcl_matrix(K_without_supports.size1(), K_without_supports.size2());

      viennacl::copy(F, vcl_rhs);
      viennacl::copy(K_without_supports, vcl_matrix);

      vcl_result_gl1 = solve(vcl_matrix, vcl_rhs, upper_tag{});
      vcl_result_gl1 = solve(vcl_matrix, vcl_rhs, lower_tag{});
      lu_factorize(vcl_matrix);
      lu_substitute(vcl_matrix, vcl_rhs);

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

      //! \todo kontrolle der auflagerreaktionen
      //    // sum H und sum V
      // }

      Log::info("auflagerreaktionen: {}", auflagerreaktionen);
      Log::info("verschiebungen: {}", verschiebungen);

      result.auflagerreaktionen.resize(n * dim);
      result.verschiebungen.resize(n * dim);

      zip([&result](auto&& tuple) {
         result.auflagerreaktionen[static_cast<int>(std::get<impl::Row>(tuple))] = std::get<1>(tuple);
      }, std::begin(festgehaltende_zeilen), std::end(festgehaltende_zeilen), std::begin(auflagerreaktionen));

      //! \todo factor k= 0.036
      zip([&result](auto&& tuple) {
         result.verschiebungen[static_cast<int>(std::get<impl::Row>(tuple))] = std::get<1>(tuple) * 0.036;
      }, std::begin(verschiebungs_zeilen), std::end(verschiebungs_zeilen), std::begin(verschiebungen));   

      //! \todo to complex!
      for (auto&& m : sys.members)
      {
         const auto[s, e] = sys.adjacency_list.at(m.id);
         const auto EA = m.E * m.A;
         const auto EA_over_l = EA / length(sys, m.id).value();
         const auto uv_in_millimeter = std::array{
            result.verschiebungen[s * dim + 0],
            result.verschiebungen[s * dim + 1],
            result.verschiebungen[e * dim + 0],
            result.verschiebungen[e * dim + 1]
         };
         //! \todo factor k= 1/10000; unit?
         const auto N = impl::normalkraft(sys, m.id, uv_in_millimeter) * EA_over_l / 10'000;
         result.normalkraefte.push_back(N);
      }
      return std::tuple{true, result};
   }
}