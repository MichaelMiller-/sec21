#include <catch.hpp>

#include <sec21/database/column_type.h>

namespace sec21::database
{
   template <>
   struct column_type<int>
   {
      static constexpr std::string_view value = "INT";
   };
   template <>
   struct column_type<std::string>
   {
      static constexpr std::string_view value = "TEXT";
   };
   template <>
   struct column_type<double>
   {
      static constexpr std::string_view value = "REAL";
   };
} // namespace sec21::database

TEST_CASE("test column type wrapper", "[sec21][database]")
{
   using namespace sec21::database;

   REQUIRE(get_column_type<int>() == "INT");
   REQUIRE(get_column_type<std::string>() == "TEXT");
   REQUIRE(get_column_type<double>() == "REAL");
}
