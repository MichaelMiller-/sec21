#pragma once

#include <sec21/units/dimensions/base_dimensions.h>
#include <sec21/units/quantity.h>

#ifdef pascal
#undef pascal
#endif

namespace sec21::units
{
   // clang-format off
   // begin-snippet: units_pressure_dimension
   using pressure = dimension<
      exponent<base_dimension_length, -1>,
      exponent<base_dimension_mass, 1>, 
      exponent<base_dimension_time, -2>>;
   // end-snippet
   // clang-format on

   // begin-snippet: units_pressure_hierachie
   struct pascal : derived_unit<pascal, pressure, base_unit>
   {
   };
   struct kilopascal : derived_unit<kilopascal, pressure, std::kilo>
   {
   };
   struct megapascal : derived_unit<megapascal, pressure, std::mega>
   {
   };
   struct gigapascal : derived_unit<gigapascal, pressure, std::giga>
   {
   };

   // stress
   struct newton_per_square_meter : derived_unit<pascal, pressure, base_unit>
   {
   };
   struct newton_per_square_millimeter : derived_unit<pascal, pressure, std::mega>
   {
   };
   struct kilonewton_per_square_meter : derived_unit<newton_per_square_meter, pressure, std::kilo>
   {
   };

   //! \todo struct psi : derived_unit<kilopascal, pressure, std::ratio<6.894757>> {};
   // end-snippet

   template <>
   struct abbreviation<pascal>
   {
      using type_t = pascal;
      static constexpr std::string_view value{"Pa"};
   };
   template <>
   struct abbreviation<kilopascal>
   {
      using type_t = kilopascal;
      static constexpr std::string_view value{"kPa"};
   };
   template <>
   struct abbreviation<megapascal>
   {
      using type_t = megapascal;
      static constexpr std::string_view value{"MPa"};
   };
   template <>
   struct abbreviation<gigapascal>
   {
      using type_t = gigapascal;
      static constexpr std::string_view value{"GPa"};
   };
   // stress
   template <>
   struct abbreviation<newton_per_square_meter>
   {
      using type_t = newton_per_square_meter;
      static constexpr std::string_view value{"N/sq_m"};
   };
   template <>
   struct abbreviation<newton_per_square_millimeter>
   {
      using type_t = newton_per_square_millimeter;
      static constexpr std::string_view value{"N/sq_mm"};
   };
   template <>
   struct abbreviation<kilonewton_per_square_meter>
   {
      using type_t = kilonewton_per_square_meter;
      static constexpr std::string_view value{"kN/sq_m"};
   };

   template <>
   struct type_info<pressure>
   {
      // clang-format off
      using valid_types_t = std::tuple<
         pascal,
         kilopascal,
         megapascal,
         gigapascal,
         newton_per_square_meter,
         newton_per_square_millimeter,
         kilonewton_per_square_meter>;
      // clang-format on
      static constexpr std::string_view name{"pressure"};
   };

   inline namespace literals
   {
      constexpr auto operator"" _Pa(unsigned long long v) noexcept { return quantity<pascal, unsigned long long>{v}; }
      constexpr auto operator"" _Pa(long double v) noexcept { return quantity<pascal, long double>{v}; }

      constexpr auto operator"" _kPa(unsigned long long v) noexcept
      {
         return quantity<kilopascal, unsigned long long>{v};
      }
      constexpr auto operator"" _kPa(long double v) noexcept { return quantity<kilopascal, long double>{v}; }

      constexpr auto operator"" _MPa(unsigned long long v) noexcept
      {
         return quantity<megapascal, unsigned long long>{v};
      }
      constexpr auto operator"" _MPa(long double v) noexcept { return quantity<megapascal, long double>{v}; }

      constexpr auto operator"" _GPa(unsigned long long v) noexcept
      {
         return quantity<gigapascal, unsigned long long>{v};
      }
      constexpr auto operator"" _GPa(long double v) noexcept { return quantity<gigapascal, long double>{v}; }

      // stress
      constexpr auto operator"" _N_per_sqm(unsigned long long v) noexcept
      {
         return quantity<newton_per_square_meter, unsigned long long>{v};
      }
      constexpr auto operator"" _N_per_sqm(long double v) noexcept
      {
         return quantity<newton_per_square_meter, long double>{v};
      }

      constexpr auto operator"" _N_per_sqmm(unsigned long long v) noexcept
      {
         return quantity<newton_per_square_millimeter, unsigned long long>{v};
      }
      constexpr auto operator"" _N_per_sqmm(long double v) noexcept
      {
         return quantity<newton_per_square_millimeter, long double>{v};
      }

      constexpr auto operator"" _kN_per_sqm(unsigned long long v) noexcept
      {
         return quantity<kilonewton_per_square_meter, unsigned long long>{v};
      }
      constexpr auto operator"" _kN_per_sqm(long double v) noexcept
      {
         return quantity<kilonewton_per_square_meter, long double>{v};
      }
   } // namespace literals
} // namespace sec21::units
