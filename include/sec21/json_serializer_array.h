#pragma once

#include <nlohmann/adl_serializer.hpp>
#include <nlohmann/json.hpp>

#include <array>

namespace nlohmann
{
   template <typename T>
   struct adl_serializer<std::array<T, 2>>
   {
      using type_t = std::array<T, 2>;

      static void to_json(json& j, type_t const& pt)
      {
         j["x"] = std::get<0>(pt);
         j["y"] = std::get<1>(pt);
      }

      static void from_json(json const& j, type_t& pt)
      {
         j.at("x").get_to(std::get<0>(pt));
         j.at("y").get_to(std::get<1>(pt));
      }
   };

   template <typename T>
   struct adl_serializer<std::array<T, 3>>
   {
      using type_t = std::array<T, 3>;

      static void to_json(json& j, type_t const& pt)
      {
         j["x"] = std::get<0>(pt);
         j["y"] = std::get<1>(pt);
         j["z"] = std::get<2>(pt);
      }
      static void from_json(json const& j, type_t& pt)
      {
         j.at("x").get_to(std::get<0>(pt));
         j.at("y").get_to(std::get<1>(pt));
         j.at("z").get_to(std::get<2>(pt));
      }
   };
} // namespace nlohmann