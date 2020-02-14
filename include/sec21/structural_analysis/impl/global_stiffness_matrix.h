#pragma once

#include <sec21/structural_analysis/impl/stiffness_matrix.h>

namespace sec21::structural_analysis::impl
{
   //! \brief "globale" Systemsteifigkeitsmatrix (K_f) ohne Berücksichtigung der Auflagerbedingungen
   template <typename System>
   [[nodiscard]] auto global_stiffness_matrix(System const& sys) -> boost::numeric::ublas::matrix<typename System::precision_t>
   {
      using namespace boost::numeric;

      constexpr auto dim = System::node_t::dimension_v;
      static_assert(dim == 2, "currently only works with 2D systems");

      const auto n = std::size(sys.nodes) * dim;
      ublas::matrix<typename System::precision_t> result(n, n, 0);

      //! \todo parallelize 
      for (auto const& member : sys.members)
         result += stiffness_matrix(sys, member.id);

      return result;
   }
}