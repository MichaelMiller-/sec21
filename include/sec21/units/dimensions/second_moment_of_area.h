#pragma once

#include <sec21/units/dimensions/base_dimensions.h>
#include <sec21/units/quantity.h>

namespace sec21::units
{
   // dimension
   using second_moment_of_area = dimension<exponent<base_dimension_length, 4>>;

   // units
   struct meters_to_the_fourth_power : derived_unit<meters_to_the_fourth_power, second_moment_of_area, si_tag>
   {
   };
   struct centimeters_to_the_fourth_power
       : derived_unit<centimeters_to_the_fourth_power, second_moment_of_area, std::ratio<1, 100'000'000>>
   {
   };
   struct millimeters_to_the_fourth_power
       : derived_unit<millimeters_to_the_fourth_power, second_moment_of_area, std::ratio<1, 1'000'000'000'000>>
   {
   };

   template <>
   struct abbreviation<meters_to_the_fourth_power>
   {
      using type_t = meters_to_the_fourth_power;
      static constexpr std::string_view value{"m4"};
   };
   template <>
   struct abbreviation<millimeters_to_the_fourth_power>
   {
      using type_t = millimeters_to_the_fourth_power;
      static constexpr std::string_view value{"mm4"};
   };
   template <>
   struct abbreviation<centimeters_to_the_fourth_power>
   {
      using type_t = centimeters_to_the_fourth_power;
      static constexpr std::string_view value{"cm4"};
   };

   template <>
   struct type_info<second_moment_of_area>
   {
      using valid_types_t =
         std::tuple<meters_to_the_fourth_power, millimeters_to_the_fourth_power, centimeters_to_the_fourth_power>;
      static constexpr std::string_view name{"second moment of area"};
   };

   inline namespace literals
   {
      constexpr auto operator"" _mm4(unsigned long long v) noexcept
      {
         return quantity<millimeters_to_the_fourth_power, unsigned long long>{v};
      }
      constexpr auto operator"" _mm4(long double v) noexcept
      {
         return quantity<millimeters_to_the_fourth_power, long double>{v};
      }

      constexpr auto operator"" _cm4(unsigned long long v) noexcept
      {
         return quantity<centimeters_to_the_fourth_power, unsigned long long>{v};
      }
      constexpr auto operator"" _cm4(long double v) noexcept
      {
         return quantity<centimeters_to_the_fourth_power, long double>{v};
      }

      constexpr auto operator"" _m4(unsigned long long v) noexcept
      {
         return quantity<meters_to_the_fourth_power, unsigned long long>{v};
      }
      constexpr auto operator"" _m4(long double v) noexcept
      {
         return quantity<meters_to_the_fourth_power, long double>{v};
      }
   } // namespace literals
} // namespace sec21::units
