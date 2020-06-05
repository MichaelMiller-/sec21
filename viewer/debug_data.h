#pragma once

#include <vector>
#include <string>

namespace sec21::viewer
{
   struct debug_data
   {
      std::vector<std::string> example_files{};
      std::vector<std::string> example_load_files{};
   };
}