#pragma once

#include <sec21/event/mouse.h>

#include <nlohmann/json.hpp>

#include <string_view>

namespace sec21::event
{
   struct mouse_button
   {
      static constexpr std::string_view name{ "mouse" };

      int button{};
      mouse position{};
   };

   void to_json(nlohmann::json& j, mouse_button const& obj)
   {
      j = nlohmann::json{
         { "button",  obj.button },
         { "position",  obj.position }
      };
   }
   void from_json(nlohmann::json const& j, mouse_button& obj)
   {
      j.at("button").get_to(obj.button);
      j.at("position").get_to(obj.position);
   }
}
