#include <catch2/catch_test_macros.hpp>

#include <sec21/transform.h>

TEST_CASE("convert a std::tuple into another std::tuple", "[sec21]")
{
   using namespace sec21;

   auto input = std::make_tuple(1, 2, 3, 4.44);
   auto result = transform(input, [](auto v) { return v * 2; });

   REQUIRE(std::tuple_size_v<decltype(result)> == 4);
   REQUIRE(std::get<0>(result) == 2);
   REQUIRE(std::get<1>(result) == 4);
   REQUIRE(std::get<2>(result) == 6);
   REQUIRE(std::get<3>(result) == 8.88);
}
