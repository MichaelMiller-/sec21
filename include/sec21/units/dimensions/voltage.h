#pragma once

#include <sec21/units/dimensions/base_dimensions.h>
#include <sec21/units/quantity.h>

namespace sec21::units
{
   using voltage = dimension<exponent<base_dimension_length, 3>>;

   struct volt : coherent_derived_unit<volt, voltage> {};

   inline namespace literals
   {
      constexpr auto operator "" _V(unsigned long long v) noexcept  { return quantity<volt, unsigned long long>{ v }; }
   }

#ifdef __cpp_concepts

#endif
}
