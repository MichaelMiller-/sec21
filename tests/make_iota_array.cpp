#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <sec21/make_iota_array.h>

#include <algorithm>

TEST_CASE("make iota array", "[core]")
{
   using namespace sec21;

   SECTION("simple test")
   {
      auto result = make_iota_array<int, 4>();
      const decltype(result) expected{ 0,1,2,3 };
      REQUIRE(std::equal(std::begin(result), std::end(result), std::begin(expected)) == true);
   }
   SECTION("with offset")
   {
      auto result = make_iota_array<int, 4>(2);
      const decltype(result) expected{ 2,3,4,5 };
      REQUIRE(std::equal(std::begin(result), std::end(result), std::begin(expected)) == true);
   }
}   
