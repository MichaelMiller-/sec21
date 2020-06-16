#pragma once

#include <sec21/units/dimensions/base_dimensions.h>
#include <sec21/units/quantity.h>

namespace sec21::units
{
   using electric_charge = dimension<exponent<base_dimension_time, 1>, exponent<base_dimension_current, 1>>;

   struct coulomb : derived_unit<coulomb, electric_charge, base_unit> {};

   inline namespace literals
   {
      constexpr auto operator "" _C(unsigned long long v) noexcept  { return quantity<coulomb, unsigned long long>{ v }; }
   }
}
