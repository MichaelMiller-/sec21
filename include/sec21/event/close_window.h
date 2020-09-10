#pragma once

#include <nlohmann/json.hpp>

#include <string_view>

namespace sec21::event
{
   struct close_window
   {
      static constexpr std::string_view name{ "close window" };
   };


   void to_json(nlohmann::json& j, close_window const& obj)
   {
      j = nlohmann::json{ { obj.name, nlohmann::json::object() } };
   }

   void from_json(nlohmann::json const& j, close_window& obj)
   {
      j.at(obj.name.data());
   }
}
