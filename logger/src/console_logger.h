#pragma once
#include "base_logger.h"

namespace sec21::log
{
   struct console_logger : base_logger
   {
      console_logger();

      void log(entry const& e) override;
   };
} // namespace sec21::log