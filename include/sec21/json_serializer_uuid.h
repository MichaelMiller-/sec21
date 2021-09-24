#pragma once

#include <nlohmann/json.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/string_generator.hpp>

namespace nlohmann
{
   template <>
   struct adl_serializer<boost::uuids::uuid>
   {
      using type_t = boost::uuids::uuid;

      static void to_json(json& j, type_t const& obj)
      {
         j = to_string(obj);
      }
      static void from_json(json const& j, type_t& obj)
      {
         static boost::uuids::string_generator string_gen{};
         obj = string_gen(j.get<std::string>());
      }
   };
}
