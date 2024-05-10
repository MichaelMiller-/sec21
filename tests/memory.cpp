#include <catch.hpp>

#include <sec21/literals/memory.h>
#include <sec21/memory.h>

TEST_CASE("memory class", "[sec21]")
{
   using namespace sec21;
   using namespace sec21::literals;

   SECTION("addition")
   {
      REQUIRE(memory{1} + memory{2} == memory{3});
      REQUIRE(4_kiB + std::byte{4} == memory{4100});
   }
   SECTION("subtraction")
   {
      REQUIRE(memory{2} - memory{1} == memory{1});
      REQUIRE(4_kiB - std::byte{4} == memory{4092});
   }
   SECTION("test safety check of subtraction operator")
   {
      REQUIRE_THROWS([] { auto result = memory{2} - memory{10}; }());
      // REQUIRE_THROWS([] { auto result = memory{2} - std::byte{10}; }());
   }
   SECTION("multiplication")
   {
      REQUIRE(memory{2} * memory{1} == 2_B);
      REQUIRE(4_kiB * std::byte{2} == memory{8192});
   }
   SECTION("test formatter")
   {
      SECTION("default") { REQUIRE(std::format("{}", memory{1024}) == "1024B"); }

      SECTION("format output to a human readable format")
      {
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