#pragma once

#include "invoke.h"

#include <sec21/validate.h>

#include <nlohmann/json.hpp>

#include <array>
#include <string_view>

namespace sec21::editor
{
   struct position
   {
      float x;
      float y;
   };

   auto to_array(position const& pos) noexcept 
   {
      return std::array{ pos.x, pos.y };
   }

   void to_json(nlohmann::json &j, position const &obj)
   {
      j = nlohmann::json{
         {"x", obj.x},
         {"y", obj.y}
      };
   }
   void from_json(nlohmann::json const &j, position &obj)
   {
      j.at("x").get_to(obj.x);
      j.at("y").get_to(obj.y);
   }

   using command_add_node_t = invoke<position>;
}

namespace sec21
{
   template <>
   struct validate<editor::command_add_node_t>
   {
      std::optional<editor::command_add_node_t> operator()(std::string_view input) const noexcept
      {
         try {
            const auto j = nlohmann::json::parse(input);
            const auto result = j.get<editor::command_add_node_t>();

            if (result.command != "add_node")
               return std::nullopt;
            return result;            
         }
         catch(...) {
            return std::nullopt;
         }
         return std::nullopt;
      }
   };
}
