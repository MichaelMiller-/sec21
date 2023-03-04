#include <catch.hpp>

#include <sec21/database/constraints.h>

TEST_CASE("concat column constrains", "[sec21][database]")
{
   using namespace sec21::database;

   SECTION("empty list")
   {
      using input_t = std::tuple<>;
      REQUIRE(std::empty(concat_constraints<input_t>()));
   }
   SECTION("list with one element")
   {
      using input_t = std::tuple<not_null>;
      REQUIRE(concat_constraints<input_t>() == "NOT NULL");
   }
   SECTION("list with two elements")
   {
      using input_t = std::tuple<primary_key, not_null>;
      REQUIRE(concat_constraints<input_t>() == "PRIMARY KEY NOT NULL");
   }
}
