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

   struct pascal : derived_unit<pascal, pressure, base_unit> {};
   struct kilopascal : derived_unit<kilopascal, pressure, std::kilo> {};
   struct megapascal : derived_unit<megapascal, pressure, std::mega> {};

   // stress
   struct newton_per_square_meter : derived_unit<pascal, pressure, base_unit> {};
   struct kilonewton_per_square_meter : derived_unit<newton_per_square_meter, pressure, std::kilo> {};

   // struct psi : derived_unit<kilopascal, pressure, std::ratio<6.894757>> {};

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
   // stress
   template <>
   struct abbreviation<newton_per_square_meter>
   {
      using type_t = newton_per_square_meter;
      constexpr static std::string_view value{"N/sq_m"};
   };   
   template <>
   struct abbreviation<kilonewton_per_square_meter>
   {
      using type_t = kilonewton_per_square_meter;
      constexpr static std::string_view value{"kN/sq_m"};
   };

   template <>
   struct type_info<pressure>
   {
      using valid_types_t = std::tuple<pascal, kilopascal, megapascal, newton_per_square_meter, kilonewton_per_square_meter>;
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

      // stress
      constexpr auto operator "" _N_per_sqm(unsigned long long v) noexcept  { return quantity<newton_per_square_meter, unsigned long long>{ v }; }
      constexpr auto operator "" _N_per_sqm(long double v) noexcept         { return quantity<newton_per_square_meter, long double>{ v }; }

      constexpr auto operator "" _kN_per_sqm(unsigned long long v) noexcept  { return quantity<kilonewton_per_square_meter, unsigned long long>{ v }; }
      constexpr auto operator "" _kN_per_sqm(long double v) noexcept         { return quantity<kilonewton_per_square_meter, long double>{ v }; }      
   }
}
