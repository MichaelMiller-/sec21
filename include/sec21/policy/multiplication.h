#pragma once

namespace sec21::policy
{
   template <typename T>
   struct multiplication
   {
      friend constexpr T& operator *= (T& lhs, T const& rhs) noexcept
      {
         using type = typename T::underlying_t;
         static_cast<type&>(lhs) *= static_cast<type const&>(rhs);
         return lhs;
      }
      friend constexpr T operator * (T const& lhs, T const& rhs) noexcept
      {
         using type = typename T::underlying_t;
         return T(static_cast<type const&>(lhs) * static_cast<type const&>(rhs));
      }
   };    
}