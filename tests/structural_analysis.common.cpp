#include <catch.hpp>
#include "approx_equal.h"

#include <sec21/units.h>
#include <sec21/structural_analysis/common.h>

TEST_CASE("EA over length", "[sec21][structural_analysis][common]")
{
   using namespace sec21::structural_analysis;
   using namespace sec21::units::literals;

	const auto E = 210000_kPa;
	const auto A = 0.004_sq_m;
   const auto l = 5.0_m;

   auto result = EA_over_l<double>(E, A, l);
   REQUIRE(result.value() == Approx(168.0)); // kN/sq_m
}
