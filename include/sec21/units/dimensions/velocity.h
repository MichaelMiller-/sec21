#pragma once

#include <sec21/units/dimensions/base_dimensions.h>
#include <sec21/units/quantity.h>

namespace sec21::units
{
   using velocity = dimension<exponent<base_dimension_length, 1>, exponent<base_dimension_time, -1>>;

   struct meter_per_second : coherent_derived_unit<meter_per_second, velocity, si_tag> {};
   struct kilometer_per_hour : derived_unit<kilometer_per_hour, velocity, std::ratio<3600>> {};

   inline namespace literals
   {
      constexpr auto operator "" _mps(unsigned long long v) noexcept  { return quantity<meter_per_second, unsigned long long>{ v }; }
      constexpr auto operator "" _kmph(unsigned long long v) noexcept  { return quantity<kilometer_per_hour, unsigned long long>{ v }; }
   }

#ifdef __cpp_concepts

#endif
}
