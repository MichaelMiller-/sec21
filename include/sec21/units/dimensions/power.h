#pragma once

#include <sec21/units/dimensions/base_dimensions.h>
#include <sec21/units/quantity.h>

namespace sec21::units
{
   // clang-format off
   using power = dimension<
      exponent<base_dimension_length, 2>,
      exponent<base_dimension_mass, 1>,
      exponent<base_dimension_time, -3>>;
   // clang-format on

   struct watt : coherent_derived_unit<watt, power> {};
   struct milliwatt : derived_unit<milliwatt, power, std::milli> {};
   struct kilowatt : derived_unit<kilowatt, power, std::kilo> {};
   struct megawatt : derived_unit<megawatt, power, std::mega> {};
   struct gigawatt : derived_unit<gigawatt, power, std::giga> {};

   inline namespace literals
   {
      constexpr auto operator "" _W(unsigned long long v) noexcept  { return quantity<watt, unsigned long long>{ v }; }
      constexpr auto operator "" _kW(unsigned long long v) noexcept  { return quantity<kilowatt, unsigned long long>{ v }; }
      constexpr auto operator "" _mW(unsigned long long v) noexcept  { return quantity<milliwatt, unsigned long long>{ v }; }
      constexpr auto operator "" _MW(unsigned long long v) noexcept  { return quantity<megawatt, unsigned long long>{ v }; }
      constexpr auto operator "" _GW(unsigned long long v) noexcept  { return quantity<gigawatt, unsigned long long>{ v }; }
   }
}
