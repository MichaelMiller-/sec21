#include <catch.hpp>

#include <sec21/memory.h>
#include <sec21/literals/memory.h>

TEST_CASE("memory class", "[sec21]")
{
   using namespace sec21;

   SECTION("addition")
   {
      memory m1{1};
      memory m2{2};
      REQUIRE(m1 + m2 == memory{3});
   }
   SECTION("subtraction")
   {
      memory m1{1};
      memory m2{2};
      REQUIRE(m2 - m1 == memory{1});
   }
   SECTION("test safety check of subtraction operator")
   {
      REQUIRE_THROWS([] {
         memory m1{1};
         memory m2{2};
         auto result = m1 - m2;
      }());
   }
   SECTION("multiplication")
   {
      memory m1{1};
      memory m2{2};
      REQUIRE(m1 * m2 == memory{2});
   }
   SECTION("test formatter")
   {
      SECTION("default") { REQUIRE(std::format("{}", memory{1024}) == "1024B"); }

      SECTION("format output to a human readable format")
      {
         using namespace sec21::literals;

         REQUIRE(std::format("{:h}", memory{1_kiB}) == "1.02kB");
         REQUIRE(std::format("{:h}", memory{1_MiB}) == "1.05MB");
         REQUIRE(std::format("{:h}", memory{1_GiB}) == "1.07GB");
         REQUIRE(std::format("{:h}", memory{1_TiB}) == "1.10TB");
         REQUIRE(std::format("{:h}", memory{1_PiB}) == "1.13PB");
      }
      SECTION("human readable format with precision")
      {
         REQUIRE(std::format("{:.0h}", memory{1024 * 1024}) == "1MB");
         REQUIRE(std::format("{:.1h}", memory{1024 * 1024}) == "1.0MB");
         REQUIRE(std::format("{:.4h}", memory{1024 * 1024}) == "1.0486MB");
         REQUIRE(std::format("{:.h}", memory{1024 * 1024}) == "1.05MB");
      }
   }
}
