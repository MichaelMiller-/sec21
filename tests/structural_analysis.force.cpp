#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <sec21/structural_analysis/force.h>

TEST_CASE("2D force", "[force]")
{
   using namespace sec21::structural_analysis;
   using namespace sec21::unit::literals;

   using force_t = force_2D_t;

   const force_t f1{ { 3.0_kN, 1.0_kN } };
   const force_t f2{ { 0.0, 10.0 } };
   const force_t f3{ { 10.0_N, 4.0_MN } };

   SECTION("equal test")
   {
      REQUIRE(f1 == f1);
      REQUIRE(f2 == f2);
      REQUIRE(f3 == f3);
      REQUIRE(f1 != f2);
      REQUIRE(f1 != f3);
      REQUIRE(f2 != f3);
   }
   SECTION("addition")
   {
      REQUIRE((f1 + f1) == force_t{ { 6.0_kN, 2.0_kN} });
      REQUIRE((f2 + f2) == force_t{ { 0.0_kN, 20.0_kN} });
      REQUIRE((f3 + f3) == force_t{ { 0.020_kN, 8000.0_kN} });
      REQUIRE((f1 + f2) == force_t{ { 3.0_kN, 11.0_kN} });
      REQUIRE((f1 + f3) == force_t{ { 3.010_kN, 4001.0_kN} });
      REQUIRE((f2 + f3) == force_t{ { 0.010_kN, 4010.0_kN} });
   }
   SECTION("superposition principle from empty input")
   {
      std::vector<force_t> input;
      const auto expected = force_t{ { 0.0, 0.0 } };
      REQUIRE(superposition(std::begin(input), std::end(input)) == expected);
   }
   SECTION("superposition principle")
   {
      std::vector<force_t> input{ f1, f2, f3 };
      const auto expected = force_t{ { 3.010_kN, 4011.0_kN } };
      REQUIRE(superposition(std::begin(input), std::end(input)) == expected);
   }
}

TEST_CASE("3D force", "[force]")
{
   using namespace sec21::structural_analysis;
   using namespace sec21::unit::literals;

   using force_t = force_3D_t;

   const force_t f1{ { 3.0_kN, 1.0_kN, 5.2_kN } };
   const force_t f2{ { 0.0, 10.0, 0.0 } };
   const force_t f3{ { 10.0_N, 4.0_MN, 42.3_kN } };

   SECTION("equal test")
   {
      REQUIRE(f1 == f1);
      REQUIRE(f2 == f2);
      REQUIRE(f3 == f3);
      REQUIRE(f1 != f2);
      REQUIRE(f1 != f3);
      REQUIRE(f2 != f3);
   }
   SECTION("addition")
   {
      REQUIRE((f1 + f1) == force_t{ { 6.0_kN, 2.0_kN, 10.4_kN } });
      REQUIRE((f2 + f2) == force_t{ { 0.0_kN, 20.0_kN, 0.0 } });
      REQUIRE((f3 + f3) == force_t{ { 0.020_kN, 8000.0_kN, 84.6_kN } });
      REQUIRE((f1 + f2) == force_t{ { 3.0_kN, 11.0_kN, 5.2_kN } });
      REQUIRE((f1 + f3) == force_t{ { 3.010_kN, 4001.0_kN, 47.5_kN } });
      REQUIRE((f2 + f3) == force_t{ { 0.010_kN, 4010.0_kN, 42.3_kN } });
   }
   SECTION("superposition principle from empty input")
   {
      std::vector<force_t> input;
      const auto expected = force_t{ { 0.0, 0.0, 0.0 } };
      REQUIRE(superposition(std::begin(input), std::end(input)) == expected);
   }
   SECTION("superposition principle")
   {
      std::vector<force_t> input{ f1, f2, f3 };
      const auto expected = force_t{ { 3.010_kN, 4011.0_kN, 47.5_kN } };
      REQUIRE(superposition(std::begin(input), std::end(input)) == expected);
   }
}
