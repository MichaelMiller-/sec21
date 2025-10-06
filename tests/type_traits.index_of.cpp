#include <catch2/catch_test_macros.hpp>

#include <sec21/type_traits/index_of.h>

TEST_CASE("get the index of a type in a list", "[sec21][type_traits]")
{
   using namespace sec21::type_traits;
   STATIC_REQUIRE(index_of<int, int, float, char>::value == 0);
   STATIC_REQUIRE(index_of<float, int, float, char>::value == 1);
   STATIC_REQUIRE(index_of<char, int, float, char>::value == 2);
   STATIC_REQUIRE(index_of<double, int, float, char>::value == -1);
}