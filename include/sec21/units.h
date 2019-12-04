#pragma once

#include <sec21/units/quantity.h>
// SI-units
#include <sec21/units/dimensions/acceleration.h>
#include <sec21/units/dimensions/area.h>
#include <sec21/units/dimensions/current.h>
#include <sec21/units/dimensions/electric_charge.h>
#include <sec21/units/dimensions/energy.h>
#include <sec21/units/dimensions/force.h>
#include <sec21/units/dimensions/frequency.h>
#include <sec21/units/dimensions/length.h>
#include <sec21/units/dimensions/luminous_intensity.h>
#include <sec21/units/dimensions/mass.h>
#include <sec21/units/dimensions/power.h>
#include <sec21/units/dimensions/pressure.h>
#include <sec21/units/dimensions/substance.h>
#include <sec21/units/dimensions/temperature.h>
#include <sec21/units/dimensions/time.h>
#include <sec21/units/dimensions/velocity.h>
#include <sec21/units/dimensions/volume.h>
#include <sec21/units/dimensions/voltage.h>

namespace sec21::units
{
   inline namespace constants
   {
      // gravity acceleration
      constexpr auto G = 9.80665_mps_sq;
   }
}
