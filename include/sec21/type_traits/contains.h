#pragma once

#include <type_traits>

namespace sec21::type_traits
{
   template <typename T, typename... Elements>
   struct contains : std::disjunction<std::is_same<T, Elements>...>
   {};

   template <typename T, template <typename...> class List, typename... Elements>
   struct contains<T, List<Elements...>> : std::disjunction<std::is_same<T, Elements>...>
   {};

   template <typename T, typename... Elements>
   inline constexpr bool contains_v = contains<T, Elements...>::value;

} // namespace sec21::type_traits
