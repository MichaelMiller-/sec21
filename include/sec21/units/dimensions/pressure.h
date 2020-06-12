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
   struct kilopascal : derived_unit<kilopascal, pressure, std::kilo> {};
   struct megapascal : derived_unit<megapascal, pressure, std::mega> {};

   template <>
   struct abbreviation<pascal>
   {
      using type_t = pascal;
      constexpr static std::string_view value{"Pa"};
   };
   template <>
   struct abbreviation<kilopascal>
   {
      using type_t = kilopascal;
      constexpr static std::string_view value{"kPa"};
   };
   template <>
   struct abbreviation<megapascal>
   {
      using type_t = megapascal;
      constexpr static std::string_view value{"MPa"};
   };

   template <>
   struct type_info<pressure>
   {
      using valid_types_t = std::tuple<pascal, kilopascal, megapascal>;
      constexpr static std::string_view name{"pressure"};
   };

   inline namespace literals
   {
      constexpr auto operator "" _Pa(unsigned long long v) noexcept  { return quantity<pascal, unsigned long long>{ v }; }
      constexpr auto operator "" _Pa(long double v) noexcept         { return quantity<pascal, long double>{ v }; }

      constexpr auto operator "" _kPa(unsigned long long v) noexcept  { return quantity<kilopascal, unsigned long long>{ v }; }
      constexpr auto operator "" _kPa(long double v) noexcept         { return quantity<kilopascal, long double>{ v }; }

      constexpr auto operator "" _MPa(unsigned long long v) noexcept  { return quantity<megapascal, unsigned long long>{ v }; }
      constexpr auto operator "" _MPa(long double v) noexcept         { return quantity<megapascal, long double>{ v }; }
   }
}
