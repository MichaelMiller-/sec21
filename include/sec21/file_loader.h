#pragma once

#include <nlohmann/json.hpp>

#include <filesystem>
#include <fstream>

namespace sec21
{
   template <typename T>
   auto load_from_json(std::filesystem::path const& filename) -> T
   {
      if (exists(filename)) {
         std::ifstream ifs{filename};
         nlohmann::json j{};
         ifs >> j;
         return j.get<T>();
      }
      //! \todo exception handling
      return {};
   }
} // namespace sec21
