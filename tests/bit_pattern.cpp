#include <catch.hpp>

#include <sec21/bit_pattern.h>

TEST_CASE("match bits via a pattern", "[sec21][core]")
{
   using namespace sec21;

#if __cpp_consteval >= 201811L
   STATIC_REQUIRE(0b1111111u == bit_pattern("1111111"));
   STATIC_REQUIRE(0b1101010u == bit_pattern("11xxx10"));
   STATIC_REQUIRE(0b1111111u == bit_pattern("1111111"));
   STATIC_REQUIRE(0b0000000u == bit_pattern("0000000"));
   STATIC_REQUIRE(0b1101010u == bit_pattern("xxxxxxx"));
   STATIC_REQUIRE(0b1101010u == bit_pattern("xxx10xx"));
#else
   REQUIRE(0b1111111u == bit_pattern("1111111"));
   REQUIRE(0b1101010u == bit_pattern("11xxx10"));
   REQUIRE(0b1111111u == bit_pattern("1111111"));
   REQUIRE(0b0000000u == bit_pattern("0000000"));
   REQUIRE(0b1101010u == bit_pattern("xxxxxxx"));
   REQUIRE(0b1101010u == bit_pattern("xxx10xx"));
#endif
}
