#pragma once

#include <sec21/units/dimensions/base_dimensions.h>
#include <sec21/units/quantity.h>

namespace sec21::units
{
   using volume = dimension<exponent<base_dimension_length, 3>>;

   struct cubic_meter : coherent_derived_unit<cubic_meter, volume> {};
   struct cubic_millimeter : derived_unit<cubic_millimeter, volume, std::milli> {};
   struct cubic_centimeter : derived_unit<cubic_centimeter, volume, std::centi> {};
   struct cubic_kilometer : derived_unit<cubic_kilometer, volume, std::kilo> {};

   inline namespace literals
   {
      constexpr auto operator "" _cub_m(unsigned long long v) noexcept  { return quantity<cubic_meter, unsigned long long>{ v }; }
      constexpr auto operator "" _cub_mm(unsigned long long v) noexcept  { return quantity<cubic_millimeter, unsigned long long>{ v }; }
      constexpr auto operator "" _cub_cm(unsigned long long v) noexcept  { return quantity<cubic_centimeter, unsigned long long>{ v }; }
      constexpr auto operator "" _cub_km(unsigned long long v) noexcept  { return quantity<cubic_kilometer, unsigned long long>{ v }; }      
   }

#ifdef __cpp_concepts

#endif
}
