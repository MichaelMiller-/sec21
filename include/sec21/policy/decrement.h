#pragma once

namespace sec21::policy
{
   template <typename T>
   struct decrement
   {
      constexpr T& operator -- () noexcept
      {
         using type = typename T::underlying_t;
         --static_cast<type&>(static_cast<T&>(*this));
         return static_cast<T&>(*this);
      }
      constexpr T operator -- (int) noexcept
      {
         auto result = static_cast<T&>(*this);
         --*this;
         return result;
      }
   };
}