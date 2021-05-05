#pragma once

#include "invoke.h"

#include <sec21/validate.h>

#include <nlohmann/json.hpp>

#include <string>
#include <string_view>
#include <optional>

namespace sec21::editor
{
   using command_loadfile_t = invoke<std::string>;
}

namespace sec21
{
   template <>
   struct validate<editor::command_loadfile_t>
   {
      std::optional<editor::command_loadfile_t> operator()(std::string_view input) const noexcept
      {
         try {
            auto const j = nlohmann::json::parse(input);
            auto const result = j.get<editor::command_loadfile_t>();

            if (result.command != "loadfile")
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
