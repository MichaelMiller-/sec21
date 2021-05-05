#pragma once

#include <ostream> 

namespace sec21::policy
{
   template <typename T>
   struct print
   {
      template <typename CharT, typename Traits>
      friend auto& operator << (std::basic_ostream<CharT, Traits>& os, T const& t) noexcept
      {
         using type = typename T::underlying_t;
         return os << static_cast<type const&>(t);
      }
   };
}