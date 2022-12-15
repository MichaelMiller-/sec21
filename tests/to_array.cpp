#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <sec21/to_array.h>

TEST_CASE("convert a std::tuple into a std::array", "[sec21][core]")
{
   using namespace sec21;

   auto input = std::make_tuple(1, 2, 3, 4.44f, 5.55, "string");
   auto result = to_array(input, [](auto v) {
      std::stringstream ss;
      ss << "value: " << v;
      return ss.str();
   });

   REQUIRE(size(result) == 6);
   REQUIRE(std::get<0>(result) == "value: 1");
   REQUIRE(std::get<1>(result) == "value: 2");
   REQUIRE(std::get<2>(result) == "value: 3");
   REQUIRE(std::get<3>(result) == "value: 4.44");
   REQUIRE(std::get<4>(result) == "value: 5.55");
   REQUIRE(std::get<5>(result) == "value: string");
}
