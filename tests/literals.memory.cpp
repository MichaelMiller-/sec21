#include <catch.hpp>

#include <sec21/literals/memory.h>

TEST_CASE("checks memory literals", "[sec21][type_traits]")
{
   using namespace sec21::literals;

   STATIC_REQUIRE(1_kB == 1'000);
   STATIC_REQUIRE(1_MB == 1'000'000);
   STATIC_REQUIRE(1_GB == 1'000'000'000);
   STATIC_REQUIRE(1_TB == 1'000'000'000'000);
   STATIC_REQUIRE(1_PB == 1'000'000'000'000'000);

   STATIC_REQUIRE(1_kiB == 1024);
   STATIC_REQUIRE(1_MiB == 1048576);
   STATIC_REQUIRE(1_GiB == 1073741824);
   STATIC_REQUIRE(1_TiB == 1099511627776);
   STATIC_REQUIRE(1_PiB == 1125899906842624);
}
