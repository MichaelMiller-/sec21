#pragma once

#include <sec21/units/dimensions/base_dimensions.h>
#include <sec21/units/quantity.h>

namespace sec21::units
{
   using luminous_intensity = dimension<exponent<base_dimension_luminous_intensity, 1>>;

   struct candela : coherent_derived_unit<candela, luminous_intensity, si_tag> {};

   inline namespace literals
   {
      constexpr auto operator "" _cd(unsigned long long v) noexcept  { return quantity<candela, unsigned long long>{ v }; }
   }

#ifdef __cpp_concepts

#endif
}
