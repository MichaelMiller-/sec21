#pragma once

#include <sec21/units/dimensions/base_dimensions.h>
#include <sec21/units/quantity.h>

namespace sec21::units
{
   // dimension
   using substance = dimension<exponent<base_dimension_substance, 1>>;

   // units
   struct mole : coherent_derived_unit<mole, substance, si_tag> {};
   
   inline namespace literals
   {
      constexpr auto operator "" _mol(unsigned long long v) noexcept  { return quantity<mole, unsigned long long>{ v }; }
   }

#ifdef __cpp_concepts

#endif
}
