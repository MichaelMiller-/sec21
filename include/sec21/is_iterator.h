#pragma once

#include <type_traits>
#include <iterator>

namespace sec21
{
   template <typename T, typename = void>
   struct is_iterator : std::false_type
   {
   };

   template <typename T>
   struct is_iterator<T, std::void_t<typename std::iterator_traits<T>::iterator_category>> : std::true_type
   {
   };

   template <typename T>
   constexpr auto is_iterator_v = is_iterator<T>::value;
} // namespace sec21
