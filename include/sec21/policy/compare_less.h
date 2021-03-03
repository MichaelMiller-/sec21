#pragma once

namespace sec21::policy
{
   template <typename T>
   struct compare_less
   {
      friend constexpr bool operator < (T const& lhs, const T& rhs) noexcept
      {
         using type = typename T::underlying_t;
         return static_cast<type const&>(lhs) < static_cast<type const&>(rhs);
      }
   };   
}