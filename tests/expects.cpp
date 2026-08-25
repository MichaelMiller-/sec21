#include <catch2/catch_test_macros.hpp>

#include <sec21/expects.h>

using namespace sec21;

TEST_CASE("expects", "[sec21]")
{
   SECTION("boolean overload")
   {
      REQUIRE_THROWS([]() {
         int i = 0;
         expects(i < 0, "test function overload");
      }());
   }
   SECTION("function overload")
   {
      REQUIRE_THROWS([]() { expects([i = 0]() { return i == 1; }, "test function overload"); }());
   }
}
