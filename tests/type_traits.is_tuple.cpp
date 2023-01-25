#include <catch.hpp>

#include <sec21/type_traits/is_tuple.h>

TEST_CASE("checks whether the type is a std::tuple", "[sec21][type_traits]")
{
   using namespace sec21::type_traits;

   STATIC_REQUIRE(is_tuple_v<std::tuple<int, float, int>> == true);
   STATIC_REQUIRE(is_tuple_v<std::tuple<>> == true);
   STATIC_REQUIRE(is_tuple_v<int> == false);
   STATIC_REQUIRE(is_tuple_v<std::string> == false);
   STATIC_REQUIRE(is_tuple_v<double> == false);
}
