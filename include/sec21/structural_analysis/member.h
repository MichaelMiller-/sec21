#pragma once

#include <sec21/units.h>

//! \todo 2019-04-23 could be removed if id is a strong_type
//! \todo 2019-04-26 first iteration use POD types -> switch to sec21::units
//! \todo 2019-05-10 intruduce local coordinate system
//! \todo 2020-01-18 descriptor_t should have an overload from invalid_id()
#include <limits>
#include <type_traits>

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

   template <typename T>
   struct is_member : std::is_same<T, member> {};

   template <typename T>
   constexpr bool is_member_v = is_member<T>::value;

   static_assert(is_member_v<member>, "should be a true");
   static_assert(is_member_v<float> == false, "should be a false");
   static_assert(is_member_v<int> == false, "should be a false");

#ifdef __cpp_concepts
   template <typename T>
   concept Member = is_member<T>::value;
#endif   
}
