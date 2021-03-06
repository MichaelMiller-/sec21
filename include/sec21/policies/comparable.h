#pragma once

#include <sec21/crtp.h>

namespace sec21
{
   template <typename T>
   [[deprecated]] struct comparable : crtp<T, comparable>
   {
      constexpr bool operator == (T const& other) const noexcept {
         return this->self().get() == other.get();
      }
      constexpr bool operator != (T const& other) const noexcept {
         return !(this->self().get() == other.get());
      }
   };
}
