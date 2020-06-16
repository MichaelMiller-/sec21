#pragma once

#include <sec21/units/dimensions/base_dimensions.h>
#include <sec21/units/quantity.h>

namespace sec21::units
{
   using mass = dimension<exponent<base_dimension_mass, 1>>;

   // mass
   struct kilogram : derived_unit<kilogram, mass, si_tag> {};
   struct gram : derived_unit<gram, mass, std::milli> {};
   struct ton : derived_unit<ton, mass, std::kilo> {};

   inline namespace literals
   {
      // mass
      constexpr auto operator "" _g(unsigned long long v) noexcept { return quantity<gram, unsigned long long>{ v }; }
      //constexpr auto operator "" _g(long double v) noexcept          -> gram<double> { return static_cast<double>(v); }

      constexpr auto operator "" _kg(unsigned long long v) noexcept { return quantity<kilogram, unsigned long long>{ v }; }
      //constexpr auto operator "" _kg(long double v) noexcept         -> kilogram<double> { return static_cast<double>(v); }

      constexpr auto operator "" _t(unsigned long long v) noexcept { return quantity<ton, unsigned long long>{ v }; }
      //constexpr auto operator "" _t(long double v) noexcept          -> ton<double> { return static_cast<double>(v); }
   }

#ifdef __cpp_concepts
   //template <typename T>
   //concept Mass = --> dimension_mass
#endif
}
