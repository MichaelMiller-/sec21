#pragma once

#include <nlohmann/json.hpp>

#include <filesystem>
#include <fstream>

namespace sec21
{
   template <typename T>
   [[nodiscard]] auto read_from_json(std::filesystem::path const& filename) noexcept -> T
   {
      T result;
      try {
         std::ifstream ifs{filename};
         const auto j = nlohmann::json::parse(ifs);
         result = j.get<T>();
      } catch (...) {
         // ignore
      }
      return result;
   }

} // namespace sec21
