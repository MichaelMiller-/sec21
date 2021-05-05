#pragma once

#include <string>

namespace sec21::editor
{
   template <typename Argument>
   struct invoke
   {
      //! \todo could also be a compiletime constant
      std::string command{};
      Argument argument{};
   };
}

#include <nlohmann/json.hpp>

namespace sec21::editor
{
   template <typename Argument>
   void to_json(nlohmann::json &j, invoke<Argument> const &obj)
   {
      j = nlohmann::json{
         {"command", obj.command},
         {"argument", obj.argument}
      };
   }
   template <typename Argument>
   void from_json(nlohmann::json const &j, invoke<Argument> &obj)
   {
      j.at("command").get_to(obj.command);
      j.at("argument").get_to(obj.argument);
   }
}
