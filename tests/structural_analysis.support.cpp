#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <sec21/structural_analysis/support.h>

TEST_CASE("structural analysis support", "[sec21][structural_analysis]")
{ 
   using namespace sec21::structural_analysis;

   STATIC_REQUIRE(static_cast<int>(Support::Roller) == 1);
   STATIC_REQUIRE(static_cast<int>(Support::Hinge) == 2);
   STATIC_REQUIRE(static_cast<int>(Support::Fixed) == 3);

   STATIC_REQUIRE(variability(Support::Roller) == 1);
   STATIC_REQUIRE(variability(Support::Hinge) == 2);
   STATIC_REQUIRE(variability(Support::Fixed) == 3);
}
