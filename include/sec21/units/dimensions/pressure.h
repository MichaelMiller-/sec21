#pragma once

#include <sec21/units/dimensions/base_dimensions.h>
#include <sec21/units/quantity.h>

#ifdef pascal
   #undef pascal
#endif

namespace sec21::units
{
   // clang-format off
   using pressure = dimension<
      exponent<base_dimension_length, -1>,
      exponent<base_dimension_mass, 1>, 
      exponent<base_dimension_time, -2>>;
   // clang-format on

   struct pascal : coherent_derived_unit<pascal, pressure> {};

   inline namespace literals
   {
      constexpr auto operator "" _Pa(unsigned long long v) noexcept  { return quantity<pascal, unsigned long long>{ v }; }
   }
}
