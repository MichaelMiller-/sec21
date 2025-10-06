#include <catch2/catch_test_macros.hpp>

#include <sec21/scope_guard.h>

TEST_CASE("increment a value in scope guard", "[sec21]")
{
   int i = 5;
   {
      const sec21::scope_guard sg([&i] { i++; });
   }
   REQUIRE(i == 6);
}

TEST_CASE("scope guard in a exception context", "[sec21]")
{
   using namespace sec21;

   int i = 5;
   try {
      const scope_guard_if_exception sge([&i] { i = 0; }); // rollback action
      const scope_guard sg([&i] { i += 2; });
      throw "any exception";
   } catch (...) {
      // do nothing
   }
   REQUIRE(i == 0);
}