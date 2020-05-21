#include <catch.hpp>

#include <sec21/structural_analysis/impl/geometry.h>

#include <boost/math/constants/constants.hpp>

#include <array>

TEST_CASE("test geometry", "[sec21][structural_analysis]") 
{
   using namespace sec21::structural_analysis::impl;
 
   using value_t = std::array<double, 2>;

   const auto half_pi = boost::math::constants::half_pi<double>();
   const auto fourth_pi = half_pi * 0.5;

   //! \todo consider signed or unsigned -> angle quadrant
   REQUIRE(angle_to_x_axis(value_t{ 0.0, 0.0 }, value_t{ 4.0, -3.0 }) == Approx(-0.64350));
   REQUIRE(angle_to_x_axis(value_t{ 0.0, 0.0 }, value_t{ 1.0, 1.0 }) == Approx(fourth_pi));
   REQUIRE(angle_to_x_axis(value_t{ 1.0, 1.0 }, value_t{ 4.0, 4.0 }) == Approx(fourth_pi));
   REQUIRE(angle_to_x_axis(value_t{ 5.0, 1.0 }, value_t{ 7.0, 4.0 }) == Approx(0.9827937232));
   REQUIRE(angle_to_x_axis(value_t{ 1.0, -1.0 }, value_t{ 3.0, -4.0 }) == Approx(-0.9827937232));
   REQUIRE(angle_to_x_axis(value_t{ 0.0, 0.0 }, value_t{ 3.0, 0.0 }) == Approx(0.0));
   REQUIRE(angle_to_x_axis(value_t{ 0.0, 0.0 }, value_t{ 0.0, 3.0 }) == Approx(half_pi));
   REQUIRE(angle_to_x_axis(value_t{ 0.0, 3.0 }, value_t{ 3.0, 0.0 }) == Approx(-fourth_pi));
   REQUIRE(angle_to_x_axis(value_t{ 3.0, 0.0 }, value_t{ 0.0, 3.0 }) == Approx(-fourth_pi));
}

