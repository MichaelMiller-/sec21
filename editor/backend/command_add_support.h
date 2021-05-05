#pragma once

#include "invoke.h"

#include <sec21/validate.h>

#include <nlohmann/json.hpp>

#include <array>
#include <string_view>

namespace sec21::editor
{
   struct support_properties
   {
      size_t node;
      bool px;
      bool py;
   };

   void to_json(nlohmann::json &j, support_properties const &obj)
   {
      j = nlohmann::json{
         {"node", obj.node},
         {"px", obj.px},
         {"py", obj.py}
      };
   }
   void from_json(nlohmann::json const &j, support_properties &obj)
   {
      j.at("node").get_to(obj.node);
      j.at("px").get_to(obj.px);
      j.at("py").get_to(obj.py);
   }

   using command_add_support_t = invoke<support_properties>;
}

namespace sec21
{
   template <>
   struct validate<editor::command_add_support_t>
   {
      std::optional<editor::command_add_support_t> operator()(std::string_view input) const noexcept
      {
         try {
            const auto j = nlohmann::json::parse(input);
            const auto result = j.get<editor::command_add_support_t>();

            if (result.command != "add_support")
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
