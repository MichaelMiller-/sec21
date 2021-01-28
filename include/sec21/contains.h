#pragma once

#include <algorithm>

namespace sec21
{
   template <typename T>
   [[deprecated("replace with std::any_of")]] 
   constexpr bool contains(T const& t, typename T::value_type const& v) noexcept
   {
      return std::find(std::begin(t), std::end(t), v) != std::end(t);
   }
}