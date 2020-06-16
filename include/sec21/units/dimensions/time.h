#pragma once

#include <sec21/units/dimensions/base_dimensions.h>
#include <sec21/units/quantity.h>

namespace sec21::units
{
   using time = dimension<exponent<base_dimension_time, 1>>;

   // time
   struct second : derived_unit<second, time, si_tag> {};
   struct nanosecond : derived_unit<nanosecond, time, std::nano> {};
   struct microsecond : derived_unit<microsecond, time, std::micro> {};
   struct millisecond : derived_unit<millisecond, time, std::milli> {};
   struct minute : derived_unit<minute, time, std::ratio<60>> {};
   struct hour : derived_unit<hour, time, std::ratio<3600>> {};

   inline namespace literals
   {
      constexpr auto operator "" _ms(unsigned long long v) noexcept { return quantity<millisecond, unsigned long long>{ v }; }

      constexpr auto operator "" _s(unsigned long long v) noexcept { return quantity<second, unsigned long long>{ v }; }
      //constexpr auto operator "" _s(long double v) noexcept        { return quantity<second, double>{ v }; }

      constexpr auto operator "" _min(unsigned long long v) noexcept { return quantity<minute, unsigned long long>{ v }; }
      //constexpr auto operator "" _min(long double v) noexcept        { return quantity<minute, double>{ v }; }

      constexpr auto operator "" _h(unsigned long long v) noexcept { return quantity<hour, unsigned long long>{ v }; }
      //constexpr auto operator "" _h(long double v) noexcept        { return quantity<hour, double>{ v }; }
   }
}
