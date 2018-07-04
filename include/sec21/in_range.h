#pragma once

namespace sec21 
{
   template <typename T>
   constexpr bool in_range(T const& value, T const& low, T const& high) noexcept {
      return value >= low && value <= high;
   }
}
