#pragma once

#include <sec21/units/dimensions/base_dimensions.h>
#include <sec21/units/quantity.h>

namespace sec21::units
{
   using acceleration = dimension<exponent<base_dimension_length, 1>, exponent<base_dimension_time, -2>>;

   struct meter_per_second_squared : derived_unit<meter_per_second_squared, acceleration, base_unit> {};

   inline namespace literals
   {
      constexpr auto operator "" _mps_sq(unsigned long long v) noexcept  { return quantity<meter_per_second_squared, unsigned long long>{ v }; }
      constexpr auto operator "" _mps_sq(long double v) noexcept  { return quantity<meter_per_second_squared, long double>{ v }; }
   }
}
