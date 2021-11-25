#pragma once

#include "base_logger.h"

namespace sec21::log
{
   class file_logger : public base_logger
   {
      std::map<int, std::vector<entry>> logfiles;

    public:
      void log(entry const& e) override;
   };

   std::vector<entry> messages_from_process(int pid);

} // namespace sec21::log