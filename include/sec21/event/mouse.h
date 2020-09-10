#pragma once

#include <nlohmann/json.hpp>

#include <string_view>

namespace sec21::event
{
   struct mouse
   {
      static constexpr std::string_view name{ "mouse" };

      int x{};
      int y{};
   };

   void to_json(nlohmann::json& j, mouse const& obj)
   {
      j = nlohmann::json{
         { "x",  obj.x },
         { "y",  obj.y }
      };
   }

   void from_json(nlohmann::json const& j, mouse& obj)
   {
      j.at("x").get_to(obj.x);
      j.at("y").get_to(obj.y);
   }
}
