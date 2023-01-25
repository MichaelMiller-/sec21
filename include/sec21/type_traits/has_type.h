#pragma once

#include <tuple>
#include <type_traits>
#include <variant>

namespace sec21::type_traits
{
   template <typename T, typename... Ts>
   struct has_type : std::disjunction<std::is_same<T, Ts>...>
   {
   };

   template <typename T, typename... Ts>
   struct has_type<T, std::tuple<Ts...>> : std::disjunction<std::is_same<T, Ts>...>
   {
   };

   template <typename T, typename... Ts>
   struct has_type<T, std::variant<Ts...>> : std::disjunction<std::is_same<T, Ts>...>
   {
   };

   template <typename T, typename... Ts>
   inline constexpr bool has_type_v = has_type<T, Ts...>::value;

} // namespace sec21::type_traits
