#pragma once

#include <string>

namespace sec21
{
   class stop_application
   {
      const std::string message;
   public:
      template <typename... Args>
      stop_application(Args &&... args) : message{ std::forward<Args>(args)... }
      {}

      [[nodiscard]] auto what() const noexcept { return message; }
   };
}