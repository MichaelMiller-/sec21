#pragma once

#include <nlohmann/json.hpp>

#include <string_view>

namespace sec21::event
{
   struct mouse_wheel
   {
      static constexpr std::string_view name{ "mousewheel" };

      //! \brief the amount scrolled horizontally, positive to the right and negative to the left
      int x{};
      //! \brief the amount scrolled vertically, positive away from the user and negative towards the user
      int y{};
   };

   void to_json(nlohmann::json& j, mouse_wheel const& obj)
   {
      j = nlohmann::json{
         { "x",  obj.x },
         { "y",  obj.y }
      };
   }

   void from_json(nlohmann::json const& j, mouse_wheel& obj)
   {
      j.at("x").get_to(obj.x);
      j.at("y").get_to(obj.y);
   }
}
