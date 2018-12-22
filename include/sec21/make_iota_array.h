#pragma once

#include <array>
#include <utility>

namespace sec21
{
   namespace detail 
   {
      template<typename T, T... Ns>
      constexpr auto make_iota_array(T const offset, std::integer_sequence<T, Ns...>) noexcept -> std::array<T, sizeof...(Ns)> 
      {
         return {{(Ns + offset)...}};
      }
   }

   template<typename T, T N>
   constexpr auto make_iota_array(T const offset = {}) noexcept 
   {
      static_assert(N >= T{}, "no negative sizes");
      return detail::make_iota_array<T>(offset, std::make_integer_sequence<T, N>{});
   }
}
