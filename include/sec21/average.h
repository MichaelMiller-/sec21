#pragma once

namespace sec21
{
   template <typename... Args>
   auto average(Args &&... args) noexcept
   {
      return (std::forward<Args>(args) + ...) / sizeof...(Args);
   }
} // namespace sec21