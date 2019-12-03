#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <sec21/contains.h>

#include <vector>

using namespace sec21;

TEST_CASE("check if sequence contains a element", "[core]")
{
   SECTION("empty sequence")
   {
      auto input = std::vector<int>{};
      // some example values
      REQUIRE(contains(input, 0) == false);
      REQUIRE(contains(input, 1) == false);
      REQUIRE(contains(input, 2) == false);
      REQUIRE(contains(input, 3) == false);
   }
   SECTION("sequence with int's")
   {
      auto input = std::vector<int>{0, 1, 2, 3, 4, 5, 6};
      REQUIRE(contains(input, 0) == true);
      REQUIRE(contains(input, 1) == true);
      REQUIRE(contains(input, 2) == true);
      REQUIRE(contains(input, 3) == true);
      REQUIRE(contains(input, 4) == true);
      REQUIRE(contains(input, 5) == true);
      REQUIRE(contains(input, 6) == true);
      REQUIRE(contains(input, 7) == false);
   }
   SECTION("sequence with string's")
   {
      auto input = std::vector<std::string>{"foo", "bar", "baz"};
      REQUIRE(contains(input, "foo") == true);
      REQUIRE(contains(input, "bar") == true);
      REQUIRE(contains(input, "baz") == true);
      REQUIRE(contains(input, "not") == false);
      REQUIRE(contains(input, "sec21") == false);
   }
}
