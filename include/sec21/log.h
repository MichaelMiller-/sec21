#pragma once

namespace sec21
{
   //! \todo remove file
   struct empty_logger
   {
      template <typename... Args>
      static void info(Args &&... args) {};

      template <typename... Args>
      static void warn(Args &&... args) {};

      template <typename... Args>
      static void error(Args &&... args) {};

      template <typename... Args>
      static void debug(Args &&... args) {};
   };
}
