#pragma once

#include <sec21/units/dimensions/base_dimensions.h>
#include <sec21/units/quantity.h>

namespace sec21::units
{
   using current = dimension<exponent<base_dimension_current, 1>>;

   struct ampere : coherent_derived_unit<ampere, current, si_tag> {};

   inline namespace literals
   {
      constexpr auto operator "" _A(unsigned long long v) noexcept  { return quantity<ampere, unsigned long long>{ v }; }
   }

#ifdef __cpp_concepts

#endif
}
