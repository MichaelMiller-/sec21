#pragma once

#include <fstream>
#include <string>
#include <filesystem>

namespace sec21::viewer
{
   static auto read_file(std::filesystem::path const& filename) -> std::string
   {
      std::ifstream ifs{ filename };
      return std::string((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
   }
}