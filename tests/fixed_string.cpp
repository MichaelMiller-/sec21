#include <catch2/catch_test_macros.hpp>

#include <sec21/fixed_string.h>

TEST_CASE("fixed_string class", "[sec21]")
{
   using namespace sec21;
   using namespace std::literals;

   SECTION("implicit conversion to std::string_view")
   {
      STATIC_REQUIRE(""sv == fixed_string(""));
      STATIC_REQUIRE("name"sv == fixed_string("name"));
   }
}
