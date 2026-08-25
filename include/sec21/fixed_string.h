#pragma once

#include <algorithm>
#include <string_view>

namespace sec21
{
   //! \brief compile time string
   //! see: http://www.open-std.org/JTC1/SC22/WG21/docs/papers/2018/p0732r0.pdf
   template <auto Size>
   struct fixed_string
   {
      char data[Size + 1]{};
      static constexpr auto length = Size;

      constexpr fixed_string(char const* str) { std::copy_n(str, Size + 1, data); }

      constexpr auto operator<=>(fixed_string const&) const = default;

      constexpr operator std::string_view() const { return {data, Size}; }
   };

   template <auto Size>
   fixed_string(const char (&)[Size]) -> fixed_string<Size - 1>;
} // namespace sec21
