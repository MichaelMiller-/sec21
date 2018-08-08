#pragma once

#include <type_traits>

#include <sec21/units/tags.h>

namespace sec21::units
{
   template <typename T>
   struct is_base_unit : std::is_base_of<detail::base_unit_tag, T>::type {};

   template <typename T>
   struct is_dimension : std::is_base_of<detail::dimension_tag, T>::type {};

   template <typename T>
   struct get_base_unit
   {
   private:
      typedef typename T::base_unit_t U;
   public:
      typedef typename std::conditional<is_dimension<U>::value, typename U::base_unit_t, U>::type type;
   };

   template <typename T, typename Category>
   struct is_category : std::integral_constant<bool, std::is_same<typename get_base_unit<T>::type, Category>::value>::type {};

   template <typename T, typename U>
   struct is_same_base_unit : std::integral_constant<bool, std::is_same<typename get_base_unit<T>::type, typename get_base_unit<U>::type>::value>::type {};
}


#if 0
//! \todo macro; macro for g++ > 6
// concepts
//#ifdef __cplusplus > 201704L
//
template <typename T>
concept BaseUnit = is_base_unit<T>::value;


template<typename T>
auto requires(T && t) -> decltype(
   concepts::valid_expr(
      // T must be able to dereference 
      *t,
      // result of increment operation must have type T &
      concepts::has_type<T &>(++t)
   ));

// has_type; same_type; model_of


template <typename T, typename U>
constexpr bool OutputIterator() {
   return concepts::models<concepts::OutputIterator, T, U>();
}

#endif