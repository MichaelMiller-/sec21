#pragma once

#include <sec21/units/quantity.h>

namespace sec21::structural_analysis
{
   template <typename Precision>
   inline auto EA_over_l(
      units::quantity<units::kilopascal, Precision> const& E,
      units::quantity<units::square_meter, Precision> const& A,
      units::quantity<units::meter, Precision> const& length)
   {
      return E * A / length;
   }
}