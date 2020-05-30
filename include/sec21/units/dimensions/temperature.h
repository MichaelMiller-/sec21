#pragma once

#include <sec21/units/dimensions/base_dimensions.h>
#include <sec21/units/quantity.h>

namespace sec21::units
{
   using temperature = dimension<exponent<base_dimension_temperature, 1>>;

   struct kelvin : coherent_derived_unit<kelvin, temperature, si_tag> {};
   //! \todo celsius : coherent_derived_unit<celsius, temperature, si_tag> {};

   inline namespace literals
   {
      constexpr auto operator "" _K(unsigned long long v) noexcept  { return quantity<kelvin, unsigned long long>{ v }; }
      constexpr auto operator "" _K(long double v) noexcept         { return quantity<kelvin, long double>{ v }; }
   }

#ifdef __cpp_concepts

#endif
}
