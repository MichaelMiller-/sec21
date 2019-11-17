#define _SILENCE_CXX17_UNCAUGHT_EXCEPTION_DEPRECATION_WARNING
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <sec21/disk_space.h>

using namespace sec21::literals;

TEST_CASE("units for memory usage", "[core]") 
{
   auto one_kb = 1024_B;
   auto one_mb = 1024_kB;
   auto one_gb = 1024_MB;
   auto one_tb = 1024_GB;

   SECTION("conversion")
   {
      REQUIRE(1_kB == one_kb);
      REQUIRE(1_MB == one_mb);
      REQUIRE(1_GB == one_gb);
      REQUIRE(1_TB == one_tb);
   }
}