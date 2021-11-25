#pragma once
#include "log/entry.h"

namespace sec21::log
{
   struct base_logger
   {
      virtual ~base_logger() = default;
      virtual void log(entry const&) = 0;

      virtual void flush() {}
      // virtual std::vector<entry> messages_from_process(int pid) const {};
   };
} // namespace sec21::log