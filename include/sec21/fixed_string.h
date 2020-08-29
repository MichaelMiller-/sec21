#pragma once

#include <algorithm>

namespace sec21
{
   //! \brief compile time string
   //! see: http://www.open-std.org/JTC1/SC22/WG21/docs/papers/2018/p0732r0.pdf
   template <auto N>
   struct fixed_string
   {
      static_assert(N > 0);
      char data[N + 1]{};

      constexpr fixed_string(const char (&str)[N]) {
         std::copy(str, str + N, data);
      }
      constexpr operator char const*() const { return data; }
   };
}
