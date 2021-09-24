#pragma once

#include <nlohmann/json.hpp>
#include <optional>

namespace nlohmann
{
   template <typename T>
   struct adl_serializer<std::optional<T>>
   {
      static void to_json(json& j, std::optional<T> const& opt)
      {
         if (opt == std::nullopt)
            j = nullptr;
         else
            j = *opt;
      }
      static void from_json(json const& j, std::optional<T>& opt)
      {
         if (j.is_null())
            opt = std::nullopt;
         else
            opt = j.get<T>();
      }
   };
}