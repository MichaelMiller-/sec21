#pragma once

#include <sec21/units/dimensions/base_dimensions.h>
#include <sec21/units/quantity.h>

namespace sec21::units
{
   using voltage = dimension<
      exponent<base_dimension_length, 2>, 
      exponent<base_dimension_mass, 1>, 
      exponent<base_dimension_time, -3>,
      exponent<base_dimension_current, -1>>;   

   struct volt : derived_unit<volt, voltage, base_unit> {};

   inline namespace literals
   {
      constexpr auto operator "" _V(unsigned long long v) noexcept  { return quantity<volt, unsigned long long>{ v }; }
   }
}
