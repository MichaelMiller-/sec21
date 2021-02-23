#pragma once

#include <nlohmann/json.hpp>

#include <string>

namespace sec21::editor
{
   enum class ResponseType { 
      error, 
      //! \deprecate content 
      content,
      model,
      load,
      result,
   };

   struct response
   {
      ResponseType type{ ResponseType::error };
      std::string message{};
   };

   NLOHMANN_JSON_SERIALIZE_ENUM( ResponseType, {
      { ResponseType::error, "error" },
      { ResponseType::content, "content" },
      { ResponseType::model, "model" },
      { ResponseType::load, "load" },
      { ResponseType::result, "result" },
   })

   void to_json(nlohmann::json &j, response const &obj)
   {
      j = nlohmann::json{
         {"type", obj.type},
         {"message", obj.message}
      };
   }
   void from_json(nlohmann::json const &j, response &obj)
   {
      j.at("type").get_to(obj.type);
      j.at("message").get_to(obj.message);
   }
}
