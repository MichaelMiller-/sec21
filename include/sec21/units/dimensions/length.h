#pragma once

#include <sec21/units/dimensions/base_dimensions.h>
#include <sec21/units/quantity.h>

namespace sec21::units
{
   // dimension
   using length = dimension<exponent<base_dimension_length, 1>>;

   // units
   struct meter : derived_unit<meter, length, si_tag>
   {
   };
   struct millimeter : derived_unit<millimeter, length, std::milli>
   {
   };
   struct centimeter : derived_unit<centimeter, length, std::centi>
   {
   };
   struct kilometer : derived_unit<kilometer, length, std::kilo>
   {
   };

   template <>
   struct abbreviation<meter>
   {
      using type_t = meter;
      static constexpr std::string_view value{"m"};
   };
   template <>
   struct abbreviation<millimeter>
   {
      using type_t = millimeter;
      static constexpr std::string_view value{"mm"};
   };
   template <>
   struct abbreviation<centimeter>
   {
      using type_t = centimeter;
      static constexpr std::string_view value{"cm"};
   };
   template <>
   struct abbreviation<kilometer>
   {
      using type_t = kilometer;
      static constexpr std::string_view value{"km"};
   };

   template <>
   struct type_info<length>
   {
      using valid_types_t = std::tuple<meter, millimeter, centimeter, kilometer>;
      static constexpr std::string_view name{"length"};
   };

   inline namespace literals
   {
      constexpr auto operator"" _mm(unsigned long long v) noexcept
      {
         return quantity<millimeter, unsigned long long>{v};
      }
      constexpr auto operator"" _mm(long double v) noexcept { return quantity<millimeter, long double>{v}; }

      constexpr auto operator"" _cm(unsigned long long v) noexcept
      {
         return quantity<centimeter, unsigned long long>{v};
      }
      constexpr auto operator"" _cm(long double v) noexcept { return quantity<centimeter, long double>{v}; }

      constexpr auto operator"" _m(unsigned long long v) noexcept { return quantity<meter, unsigned long long>{v}; }
      constexpr auto operator"" _m(long double v) noexcept { return quantity<meter, long double>{v}; }

      constexpr auto operator"" _km(unsigned long long v) noexcept
      {
         return quantity<kilometer, unsigned long long>{v};
      }
      constexpr auto operator"" _km(long double v) noexcept { return quantity<kilometer, long double>{v}; }
   } // namespace literals
} // namespace sec21::units
