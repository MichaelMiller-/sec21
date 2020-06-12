#include <catch.hpp>

#include <sec21/structural_analysis/support.h>

TEST_CASE("structural analysis support", "[sec21][structural_analysis][support]")
{ 
   using namespace sec21::structural_analysis;

   STATIC_REQUIRE(static_cast<int>(Support::Roller) == 1);
   STATIC_REQUIRE(static_cast<int>(Support::Hinge) == 2);
   STATIC_REQUIRE(static_cast<int>(Support::Fixed) == 3);
}
