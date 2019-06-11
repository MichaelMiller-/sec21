#pragma once

#include <sec21/units.h>

//! \todo 2019-04-23 could be removed if id is a strong_type
//! \todo 2019-04-26 first iteration use POD types -> switch to sec21::units
//! \todo 2019-05-10 intruduce local coordinate system
#include <limits>

namespace sec21::structural_analysis
{
   struct member
   {
      using descriptor_t = size_t;
      using precision_t = double;

      descriptor_t id{ std::numeric_limits<descriptor_t>::max() };

      double E;
      double A;      
      //unit::gigapascal<precision_t> E;
      //unit::square_meter<precision_t> A;
   };
}