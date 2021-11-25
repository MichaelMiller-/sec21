#pragma once

#include "entry.h"

#include <fmt/format.h>

#include <string_view>
#include <vector>

namespace sec21::log
{
   void info(std::string_view message, std::optional<boost::uuids::uuid> element = std::nullopt);

   template <typename... Args>
   void info(std::string_view message, Args&&... args)
   {
      // auto l = fmt::format(message, std::forward<Args>(args)...);
      // info(fmt::format(message, std::forward<Args>(args)...));
   }

   void error(entry const& e);

   void error(std::string_view message, std::optional<boost::uuids::uuid> element = std::nullopt);

   template <typename... Args>
   void error(std::string message, Args&&... args)
   {
      // error(fmt::format(message, std::forward<Args>(args)...));
   }

   void warn(entry const& e);

   void warn(std::string_view message);

   template <typename... Args>
   void warn(std::string_view message, Args&&... args)
   {
      // warn(fmt::format(message, std::forward<Args>(args)...));
   }

   void metric(std::string_view message);

   template <typename... Args>
   void metric(std::string_view message, Args&&... args)
   {
      // metric(fmt::format(message, std::forward<Args>(args)...));
   }

   void debug(std::string_view message);

   template <typename... Args>
   void debug(std::string message, Args&&... args)
   {
//      debug(fmt::format(message, std::forward<Args>(args)...));
   }

} // namespace sec21::log
