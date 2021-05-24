#pragma once

#include <sec21/crtp.h>

namespace sec21
{
   template <typename T>
   [[deprecated]] struct sortable : crtp<T, sortable>
   {
      constexpr bool operator < (T const& other) const noexcept {
         return this->self().get() < other.get();
      }
   };
}
