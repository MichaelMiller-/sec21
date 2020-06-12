#pragma once

#include <sec21/units/quantity.h>

#include <sec21/structural_analysis/concepts.h>
#include <sec21/structural_analysis/impl/geometry.h>

namespace sec21::structural_analysis
{
   //! \todo write tests
   template <typename Precision>
   auto EA_over_l(
      units::quantity<units::kilopascal, Precision> const& E,
      units::quantity<units::square_meter, Precision> const& A,
      units::quantity<units::meter, Precision> const& length)
      //! \todo -> units::quantity<units::kilonewton / units::meter, Precision>
   {
      return E * A / length;
   }
}