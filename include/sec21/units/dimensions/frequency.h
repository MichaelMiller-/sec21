#pragma once

#include <sec21/units/dimensions/base_dimensions.h>
#include <sec21/units/quantity.h>

namespace sec21::units
{
   using frequency = dimension<exponent<base_dimension_time, -1>>;

   struct hertz : coherent_derived_unit<hertz, frequency> {};
   struct millihertz : derived_unit<millihertz, frequency, std::milli> {};
   struct kilohertz : derived_unit<kilohertz, frequency, std::kilo> {};
   struct megahertz : derived_unit<megahertz, frequency, std::mega> {};
   struct gigahertz : derived_unit<gigahertz, frequency, std::giga> {};
   struct terahertz : derived_unit<terahertz, frequency, std::tera> {};

   inline namespace literals
   {
      constexpr auto operator "" _Hz(unsigned long long v) noexcept  { return quantity<hertz, unsigned long long>{ v }; }
      constexpr auto operator "" _kHz(unsigned long long v) noexcept { return quantity<kilohertz, unsigned long long>{ v }; }
      constexpr auto operator "" _GHz(unsigned long long v) noexcept { return quantity<gigahertz, unsigned long long>{ v }; }
   }

#ifdef __cpp_concepts
   //template <typename T>
   //concept Mass = --> dimension_mass
#endif
}
