#pragma once

#include <sec21/units/dimensions/base_dimensions.h>
#include <sec21/units/quantity.h>

namespace sec21::units
{
   // clang-format off
   using energy = dimension<
      exponent<base_dimension_length, 2>, 
      exponent<base_dimension_mass, 1>, 
      exponent<base_dimension_time, -2>>;
   // clang-format on

   struct joule : derived_unit<joule, energy, base_unit> {};
   struct millijoule : derived_unit<millijoule, energy, std::milli> {};
   struct kilojoule : derived_unit<kilojoule, energy, std::kilo> {};
   struct megajoule : derived_unit<megajoule, energy, std::mega> {};
   struct gigajoule : derived_unit<gigajoule, energy, std::giga> {};

   inline namespace literals
   {
      constexpr auto operator "" _J(unsigned long long v) noexcept  { return quantity<joule, unsigned long long>{ v }; }
      constexpr auto operator "" _kJ(unsigned long long v) noexcept  { return quantity<kilojoule, unsigned long long>{ v }; }
      constexpr auto operator "" _mJ(unsigned long long v) noexcept  { return quantity<millijoule, unsigned long long>{ v }; }
      constexpr auto operator "" _MJ(unsigned long long v) noexcept  { return quantity<megajoule, unsigned long long>{ v }; }
      constexpr auto operator "" _GJ(unsigned long long v) noexcept  { return quantity<gigajoule, unsigned long long>{ v }; }
   }
}
