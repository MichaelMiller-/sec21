#pragma once

#include <sec21/units/dimensions/base_dimensions.h>
#include <sec21/units/quantity.h>

namespace sec21::units
{
   using area = dimension<exponent<base_dimension_length, 2>>;

   struct square_meter : coherent_derived_unit<square_meter, area> {};
   struct square_millimeter : derived_unit<square_millimeter, area, std::micro> {};
   struct square_centimeter : derived_unit<square_centimeter, area, std::ratio<1, 10000>> {};
   struct square_kilometer : derived_unit<square_kilometer, area, std::mega> {};

   inline namespace literals
   {
      constexpr auto operator "" _sq_m(unsigned long long v) noexcept  { return quantity<square_meter, unsigned long long>{ v }; }
      constexpr auto operator "" _sq_mm(unsigned long long v) noexcept  { return quantity<square_millimeter, unsigned long long>{ v }; }
      constexpr auto operator "" _sq_cm(unsigned long long v) noexcept  { return quantity<square_centimeter, unsigned long long>{ v }; }

      constexpr auto operator "" _sq_km(unsigned long long v) noexcept  { return quantity<square_kilometer, unsigned long long>{ v }; }
      constexpr auto operator "" _sq_km(long double v) noexcept  { return quantity<square_kilometer, long double>{ v }; }

   }
}
