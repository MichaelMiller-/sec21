#pragma once

#include <algorithm>
#include <array>
#include <type_traits>

namespace sec21::type_traits
{
   template <typename T, typename... Ts>
   struct index_of
   {
      static constexpr auto value = [] {
         constexpr std::array<bool, sizeof...(Ts)> bits{{std::is_same_v<T, Ts>...}};
         const auto it = std::find(begin(bits), end(bits), true);
         return it != end(bits) ? std::distance(begin(bits), it) : -1;
      }();
   };

   template <typename T, typename... Ts>
   inline constexpr bool index_of_v = index_of<T, Ts...>::value;

} // namespace sec21::type_traits
