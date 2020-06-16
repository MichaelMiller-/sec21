#pragma once

#include <ratio>

#include <sec21/units/unit_casting.h>

namespace sec21::units
{
   // a specific physical dimension
   template <typename Dimension, typename Ratio>
   struct unit
   {
      using dimension_t = Dimension;
      using ratio_t = Ratio;
   };

   template <typename BaseUnit, typename To>
   struct cast_helper : BaseUnit 
   {
      using BaseUnit::dimension_t;
      using BaseUnit::ratio_t;
   };

   struct no_prefix;
   struct base_unit;

   template<typename Child, typename...>
   struct derived_unit;

   template <typename Child, typename Dimension>
   struct derived_unit<Child, Dimension, base_unit> : cast_helper<unit<Dimension, std::ratio<1>>, Child> {};

   template <typename Child, typename Dimension, typename Ratio>
   struct derived_unit<Child, Dimension, Ratio> : cast_helper<unit<Dimension, Ratio>, Child> {};
}
