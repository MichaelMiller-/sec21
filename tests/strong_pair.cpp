#define _SILENCE_CXX17_UNCAUGHT_EXCEPTION_DEPRECATION_WARNING
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <sec21/strong_pair.h>

TEST_CASE("strong pair", "[core]")
{
   using namespace sec21;
   
   using sample_t = strong_pair<int, int>;

   const auto id00 = sample_t{ 0, 0 };
   const auto id01 = sample_t{ 0, 1 };
   const auto id10 = sample_t{ 1, 0 };
   const auto id11 = sample_t{ 1, 1 };

   SECTION("same first and second element")
   {
      REQUIRE(id00 == id00);
      REQUIRE(id01 == id01);
      REQUIRE(id10 == id10);
      REQUIRE(id11 == id11);
   }
   SECTION("same first but different second elements")
   {
      REQUIRE((id00 == id01) == false);
      REQUIRE((id10 == id11) == false);
   }
}