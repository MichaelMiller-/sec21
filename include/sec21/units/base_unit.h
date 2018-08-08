#pragma once

#include <sec21/units/tags.h>

namespace sec21::units
{
   template<
      typename Meters = std::ratio<0>,
      typename Kilograms = std::ratio<0>,
      typename Seconds = std::ratio<0> >
   struct base_unit_t : detail::base_unit_tag
   {
      using meter_ratio = Meters;
      using kilogram_ratio = Kilograms;
      using second_ratio = Seconds;
   };
}
