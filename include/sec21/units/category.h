#pragma once

#include <sec21/units/base_unit.h>

namespace sec21::units
{
   namespace category
   {
      /// SCALAR (DIMENSIONLESS) TYPES
      //! \todo 
      //typedef base_unit_t<> scalar_unit;
      //typedef base_unit_t<> dimensionless_unit;

      /// SI BASE UNIT TYPES
      ///				         METERS			KILOGRAMS		SECONDS			
      typedef base_unit_t<std::ratio<1>>												      length;
      typedef base_unit_t<std::ratio<0>, std::ratio<1>>							      mass_unit;
      typedef base_unit_t<std::ratio<0>, std::ratio<0>, std::ratio<1>>		      time_unit;

      /// DERIVED UNITS
      ///
      //! \todo 
      //typedef base_unit_t<std::ratio<2>>												      area_unit;

      //typedef base_unit_t<std::ratio<1>, std::ratio<0>, std::ratio<-2>>				acceleration_unit;
      //typedef base_unit_t<std::ratio<1>, std::ratio<1>, std::ratio<-2>>				force_unit;
      //typedef base_unit_t<std::ratio<-1>, std::ratio<1>, std::ratio<-2>>			pressure_unit;
      //typedef base_unit_t<std::ratio<2>, std::ratio<1>, std::ratio<-2>>				energy_unit;
      //typedef base_unit_t<std::ratio<2>, std::ratio<1>, std::ratio<-3>>				power_unit;
   }
}
