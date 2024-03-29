#pragma once

#include <sec21/units/dimensions/base_dimensions.h>
#include <sec21/units/quantity.h>

namespace sec21::units
{
   // clang-format off
   using force = dimension<
      exponent<base_dimension_length, 1>, 
      exponent<base_dimension_mass, 1>,
      exponent<base_dimension_time, -2>>;
   // clang-format on

   // units
   struct newton : derived_unit<newton, force, base_unit>
   {
   };
   struct kilonewton : derived_unit<kilonewton, force, std::kilo>
   {
   };
   struct meganewton : derived_unit<meganewton, force, std::mega>
   {
   };

   template <>
   struct abbreviation<newton>
   {
      using type_t = newton;
      static constexpr std::string_view value{"N"};
   };
   template <>
   struct abbreviation<kilonewton>
   {
      using type_t = kilonewton;
      static constexpr std::string_view value{"kN"};
   };
   template <>
   struct abbreviation<meganewton>
   {
      using type_t = meganewton;
      static constexpr std::string_view value{"MN"};
   };

   template <>
   struct type_info<force>
   {
      using valid_types_t = std::tuple<newton, kilonewton, meganewton>;
      static constexpr std::string_view name{"force"};
   };

   inline namespace literals
   {
      constexpr auto operator"" _N(unsigned long long v) noexcept { return quantity<newton, unsigned long long>{v}; }
      constexpr auto operator"" _N(long double v) noexcept { return quantity<newton, long double>{v}; }

      constexpr auto operator"" _kN(unsigned long long v) noexcept
      {
         return quantity<kilonewton, unsigned long long>{v};
      }
      constexpr auto operator"" _kN(long double v) noexcept { return quantity<kilonewton, long double>{v}; }

      constexpr auto operator"" _MN(unsigned long long v) noexcept
      {
         return quantity<meganewton, unsigned long long>{v};
      }
      constexpr auto operator"" _MN(long double v) noexcept { return quantity<meganewton, long double>{v}; }
   } // namespace literals
} // namespace sec21::units
