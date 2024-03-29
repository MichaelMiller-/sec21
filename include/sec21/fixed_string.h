#pragma once

#include <algorithm>
#include <string_view>

namespace sec21
{
   //! \brief compile time string
   //! see: http://www.open-std.org/JTC1/SC22/WG21/docs/papers/2018/p0732r0.pdf
   template <auto N>
   struct fixed_string
   {
      static_assert(N > 0);
      char data[N + 1]{};

      constexpr fixed_string(const char (&str)[N + 1]) { std::copy_n(str, N + 1, data); }

      constexpr std::string_view sv() const { return {data}; }

      auto operator<=>(fixed_string const&) const = default;

      constexpr operator char const*() const { return data; }
   };

   template <auto N>
   fixed_string(const char (&str)[N]) -> fixed_string<N - 1>;
} // namespace sec21
