#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <type_traits>
#include <ratio>

#include <sec21/units/ratio.h>

TEST_CASE("units ratio", "[sec21][units]") 
{
   using namespace sec21::units;

   using two_third = std::ratio<2, 3>;
   using one_sixth = std::ratio<4, 6>;

   using r1 = common_ratio_t<two_third, one_sixth>;

   STATIC_REQUIRE(std::is_same_v<r1, std::ratio<2, 3>>);
}

