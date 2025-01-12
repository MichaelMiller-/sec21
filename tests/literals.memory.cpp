#include <catch.hpp>

#include <sec21/literals/memory.h>

TEST_CASE("checks memory literals", "[sec21][literals]")
{
   using namespace sec21;
   using namespace sec21::literals;

   STATIC_REQUIRE(memory{1_B}.bytes == 1);

   STATIC_REQUIRE(memory{1_kB}.bytes == 1'000);
   STATIC_REQUIRE(memory{1_MB}.bytes == 1'000'000);
   STATIC_REQUIRE(memory{1_GB}.bytes == 1'000'000'000);
   STATIC_REQUIRE(memory{1_TB}.bytes == 1'000'000'000'000);
   STATIC_REQUIRE(memory{1_PB}.bytes == 1'000'000'000'000'000);

   STATIC_REQUIRE(memory{1_kiB}.bytes == 1024);
   STATIC_REQUIRE(memory{1_MiB}.bytes == 1048576);
   STATIC_REQUIRE(memory{1_GiB}.bytes == 1073741824);
   STATIC_REQUIRE(memory{1_TiB}.bytes == 1099511627776);
   STATIC_REQUIRE(memory{1_PiB}.bytes == 1125899906842624);
}
