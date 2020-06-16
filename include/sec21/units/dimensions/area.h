#pragma once

#include <sec21/units/dimensions/base_dimensions.h>
#include <sec21/units/quantity.h>

#include <string_view>

namespace sec21::units
{
   using area = dimension<exponent<base_dimension_length, 2>>;

   struct square_meter : coherent_derived_unit<square_meter, area> {};
   struct square_millimeter : derived_unit<square_millimeter, area, std::micro> {};
   struct square_centimeter : derived_unit<square_centimeter, area, std::ratio<1, 10000>> {};
   struct square_kilometer : derived_unit<square_kilometer, area, std::mega> {};

   template <>
   struct abbreviation<square_meter>
   {
      using type_t = square_meter;
      constexpr static std::string_view value{"sq_m"};
   };
   template <>
   struct abbreviation<square_millimeter>
   {
      using type_t = square_millimeter;
      constexpr static std::string_view value{"sq_mm"};
   };
   template <>
   struct abbreviation<square_centimeter>
   {
      using type_t = square_centimeter;
      constexpr static std::string_view value{"sq_cm"};
   };
   template <>
   struct abbreviation<square_kilometer>
   {
      using type_t = square_kilometer;
      constexpr static std::string_view value{"sq_km"};
   };

   template <>
   struct type_info<area>
   {
      using valid_types_t = std::tuple<square_meter, square_millimeter, square_centimeter, square_kilometer>;
      constexpr static std::string_view name{"area"};
   };

   inline namespace literals
   {
      constexpr auto operator "" _sq_m(unsigned long long v) noexcept   { return quantity<square_meter, unsigned long long>{ v }; }
      constexpr auto operator "" _sq_m(long double v) noexcept         { return quantity<square_meter, long double>{ v }; }

      constexpr auto operator "" _sq_mm(unsigned long long v) noexcept  { return quantity<square_millimeter, unsigned long long>{ v }; }
      constexpr auto operator "" _sq_mm(long double v) noexcept         { return quantity<square_millimeter, long double>{ v }; }

      constexpr auto operator "" _sq_cm(unsigned long long v) noexcept  { return quantity<square_centimeter, unsigned long long>{ v }; }
      constexpr auto operator "" _sq_cm(long double v) noexcept         { return quantity<square_centimeter, long double>{ v }; }

      constexpr auto operator "" _sq_km(unsigned long long v) noexcept  { return quantity<square_kilometer, unsigned long long>{ v }; }
      constexpr auto operator "" _sq_km(long double v) noexcept         { return quantity<square_kilometer, long double>{ v }; }

   }
}
