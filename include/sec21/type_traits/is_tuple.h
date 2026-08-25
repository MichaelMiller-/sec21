#pragma once

#include <type_traits>
#include <tuple>

namespace sec21::type_traits
{
   template <typename T>
   struct is_tuple : std::false_type
   {
   };

   template <typename... Args>
   struct is_tuple<std::tuple<Args...>> : std::true_type
   {
   };

   template <typename T>
   inline constexpr bool is_tuple_v = is_tuple<T>::value;
}