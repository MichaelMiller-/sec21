#include <catch.hpp>

#include <sec21/structural_analysis/axis.h>

TEST_CASE("structural analysis axis", "[sec21][structural_analysis]")
{
   using namespace sec21::structural_analysis;

   STATIC_REQUIRE(static_cast<int>(Axis::X) == 0);
   STATIC_REQUIRE(static_cast<int>(Axis::Y) == 1);
   STATIC_REQUIRE(static_cast<int>(Axis::Z) == 2);
}

