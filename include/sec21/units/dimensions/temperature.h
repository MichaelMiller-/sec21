#pragma once

#include <sec21/units/dimensions/base_dimensions.h>
#include <sec21/units/quantity.h>

namespace sec21::units
{
   using temperature = dimension<exponent<base_dimension_temperature, 1>>;

   struct kelvin : derived_unit<kelvin, temperature, si_tag> {};
   // 1 grad celsius == 274.15 K
   //! \todo celsius : derived_unit<celsius, temperature, si_tag> {};

   template <>
   struct abbreviation<kelvin>
   {
      constexpr static std::string_view value{"K"};
   };

   template <>
   struct type_info<temperature>
   {
      using valid_types_t = std::tuple<kelvin>;
      constexpr static std::string_view name{"temperature"};
   };

   inline namespace literals
   {
      constexpr auto operator "" _K(unsigned long long v) noexcept  { return quantity<kelvin, unsigned long long>{ v }; }
      constexpr auto operator "" _K(long double v) noexcept         { return quantity<kelvin, long double>{ v }; }
   }
}
