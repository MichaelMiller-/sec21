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

   template <typename T>
   struct is_column : std::false_type {};

   template <typename T, typename... Ops>
   struct is_column<column<T, Ops...>> : std::true_type {};

   //! \todo implement tests
   // static_assert(is_column<column<int>>::value == true);
   // static_assert(is_column<column<int, primary_key>>::value == true);
   // static_assert(is_column<column<int, primary_key, not_null>>::value == true);
   // static_assert(is_column<int>::value == false);   
}