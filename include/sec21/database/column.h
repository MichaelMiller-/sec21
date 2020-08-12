#pragma once

#include <tuple>
#include <string_view>

namespace sec21::database
{
   struct primary_key {
      static inline constexpr auto value = "PRIMARY KEY";
   };
   struct not_null {
      static inline constexpr auto value = "NOT NULL";
   };

   template <typename T, typename... Ops>
   struct column
   {
      using value_t = T;
      using ops_t = std::tuple<Ops...>;
      const std::string_view name{};
   };
}