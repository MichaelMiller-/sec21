#pragma once

#include <sec21/units/dimension.h>
#include <sec21/units/unit.h>

namespace sec21::units
{
   // Basiseinheiten im SI-system
   // 
   //! \todo: using base_dimension_length = base_dimension<"length"> {}; -> need some static/compiletime text
   struct base_dimension_length : base_dimension<0> {};
   struct base_dimension_mass : base_dimension<1> {};
   struct base_dimension_time : base_dimension<2> {};
   struct base_dimension_current : base_dimension<3> {};
   struct base_dimension_temperature : base_dimension<4> {};
   struct base_dimension_substance : base_dimension<5> {};
   struct base_dimension_luminous_intensity : base_dimension<6> {};

   // tag to mark the SI base units
   using si_tag = base_unit;
}
