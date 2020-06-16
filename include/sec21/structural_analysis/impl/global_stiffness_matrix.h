#pragma once

#include <sec21/numeric/make_matrix.h>
#include <sec21/structural_analysis/impl/stiffness_matrix.h>

namespace sec21::structural_analysis::impl
{
   //! \brief "globale" Systemsteifigkeitsmatrix (K_f) ohne Berücksichtigung der Auflagerbedingungen
   template <typename Allocator, typename System>
   [[nodiscard]] auto global_stiffness_matrix(System const& sys) -> boost::numeric::ublas::matrix<typename System::precision_t>
   {
      using precision_t = typename System::precision_t;

      constexpr auto dim = System::node_t::dimension_v;
      static_assert(System::node_t::dimension_v == 2, "Only works with 2D system");

      const auto n = std::size(sys.nodes) * dim;
      auto result = numeric::make_matrix<precision_t, Allocator>(n, n, 0);

      //! \todo parallelize 
      for (auto const& member : sys.members)
         result += stiffness_matrix<Allocator>(sys, member.id);

      return result;
   }

   template <typename System>
   [[nodiscard]] auto global_stiffness_matrix(System const& sys) -> boost::numeric::ublas::matrix<typename System::precision_t>
   {
      return global_stiffness_matrix<std::allocator<typename System::precision_t>>(sys);
   }
}