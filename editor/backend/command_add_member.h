#pragma once

#include "invoke.h"

#include <sec21/validate.h>

#include <nlohmann/json.hpp>

#include <string_view>

namespace sec21::editor
{
   struct connection
   {
      size_t from;
      size_t to;
   };

   void to_json(nlohmann::json &j, connection const &obj)
   {
      j = nlohmann::json{
         {"from", obj.from},
         {"to", obj.to}
      };
   }
   void from_json(nlohmann::json const &j, connection &obj)
   {
      j.at("from").get_to(obj.from);
      j.at("to").get_to(obj.to);
   }

   using command_add_member_t = invoke<connection>;
}

namespace sec21
{
   template <>
   struct validate<editor::command_add_member_t>
   {
      std::optional<editor::command_add_member_t> operator()(std::string_view input) const noexcept
      {
         try {
            const auto j = nlohmann::json::parse(input);
            const auto result = j.get<editor::command_add_member_t>();

            if (result.command != "add_member")
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
