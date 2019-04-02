#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <sec21/math.h>

TEST_CASE("average", "[math]")
{
   SECTION("average over ints") 
   {
      REQUIRE(sec21::average(1, 2, 3, 4, 5, 6, 7, 8, 9) == 5);
   }
   SECTION("average over doubles") 
   {
      REQUIRE(sec21::average(1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0) == Approx(5.0));
   }
}

TEST_CASE("test absolute norm of an array", "[math]")
{
   const std::vector input{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
   REQUIRE(sec21::norm(std::begin(input), std::end(input)) == Approx(16.881943));
}

TEST_CASE("floating point min()", "[math]")
{
   REQUIRE(sec21::fmin(-1.5f, 1.0f, 3.0f) == -1.5f);
   REQUIRE(sec21::fmin(NAN, 1.0f, 3.0f) == 1.0f);
}