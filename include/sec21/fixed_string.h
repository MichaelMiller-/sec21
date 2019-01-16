#pragma once

#include <algorithm>

namespace sec21
{
   //! \brief compile time string
   //! probably be a part of C++20
   //! see: http://www.open-std.org/JTC1/SC22/WG21/docs/papers/2018/p0732r0.pdf
   //!
   template <typename CharT, size_t N>
   struct fixed_string
   {
      CharT data[N];

      constexpr fixed_string(const CharT (&str)[N]) {
         std::copy(str, str + N, data);
      }

      constexpr auto operator[](size_t i) const noexcept { 
         return data[i]; 
      }

      constexpr size_t size() const noexcept { 
         return N; 
      }
   };

   template <typename CharT, size_t N>
   fixed_string(const CharT (&str)[N]) -> fixed_string<CharT, N>;
}
