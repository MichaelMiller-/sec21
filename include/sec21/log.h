#pragma once

namespace sec21
{
   struct log
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
