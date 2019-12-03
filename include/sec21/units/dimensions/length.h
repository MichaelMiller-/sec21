#pragma once

#include <sec21/units/dimensions/base_dimensions.h>
#include <sec21/units/quantity.h>

namespace sec21::units
{
   // dimension
   using length = dimension<exponent<base_dimension_length, 1>>;
   
   // units
   struct meter : coherent_derived_unit<meter, length, si_tag> {};
   struct millimeter : derived_unit<millimeter, length, std::milli> {};
   struct centimeter : derived_unit<centimeter, length, std::centi> {};
   struct kilometer : derived_unit<kilometer, length, std::kilo> {};

   inline namespace literals
   {
      constexpr auto operator "" _mm(unsigned long long v) noexcept  { return quantity<millimeter, unsigned long long>{ v }; }
      constexpr auto operator "" _mm(long double v) noexcept         { return quantity<millimeter, long double>{ v }; }

      constexpr auto operator "" _cm(unsigned long long v) noexcept  { return quantity<centimeter, unsigned long long>{ v }; }
      constexpr auto operator "" _cm(long double v) noexcept         { return quantity<centimeter, long double>{ v }; }

      constexpr auto operator "" _m(unsigned long long v) noexcept   { return quantity<meter, unsigned long long>{ v }; }
      constexpr auto operator "" _m(long double v) noexcept          { return quantity<meter, long double>{ v }; }

      constexpr auto operator "" _km(unsigned long long v) noexcept  { return quantity<kilometer, unsigned long long>{ v }; }
      constexpr auto operator "" _km(long double v) noexcept         { return quantity<kilometer, long double>{ v }; }
   }

#ifdef __cpp_concepts

#endif
}
