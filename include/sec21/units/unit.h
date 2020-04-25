#pragma once

#include <ratio>

#include <sec21/units/unit_casting.h>

namespace sec21::units
{
   // a specific physical dimension
   template <typename Dimension, typename Ratio>
   struct unit // : downcast_base<unit<Dimension, Ratio>>
   {
      using dimension_t = Dimension;
      using ratio_t = Ratio;
      //! \todo abbreviation? as template parameter std::string_view
   };


   template <typename BaseType>
   struct cast_helper_base 
   {
      using base_t = BaseType;
      friend auto cast_guide(cast_helper_base);
   };

   template <typename From, typename To>
   struct cast_helper : From {
      // friend auto cast_guide(typename downcast_helper::downcast_base) { return Target(); }
   };

   struct no_prefix;
   struct base_unit;

   template <typename Child, typename Dimension, typename Prefix = no_prefix>
   struct coherent_derived_unit : cast_helper<unit<Dimension, std::ratio<1>>, Child> {};

   template<typename Child, typename...>
   struct derived_unit;

   template <typename Child, typename Dimension>
   struct derived_unit<Child, Dimension, base_unit> : cast_helper<unit<Dimension,  std::ratio<1>>, Child> {};

   template <typename Child, typename Dimension, typename Ratio>
   struct derived_unit<Child, Dimension, Ratio> : cast_helper<unit<Dimension, Ratio>, Child> {};
   // or
   // template <typename Child, typename Dimension, typename Ratio>
   // struct derived_unit : cast_helper<unit<Dimension, Ratio>, Child> {};
}
