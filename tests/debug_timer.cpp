#include <catch.hpp>

#include <sec21/debug_timer/debug_timer.h>

TEST_CASE("debug timer", "[core]")
{
   using namespace sec21;

   SECTION("simple timer")
   {
      debug_timer::start("main");
      {
         {
            auto v = debug_timer::scope("scope 1");
         }
         {
            auto v = debug_timer::scope("scope 2");
         }
      }
      for (auto i = 0; i < 4; ++i) {
         auto v = debug_timer::scope("scope 3");
      }
      auto result = debug_timer::measurement{};
      debug_timer::stop([&result](auto e) { result = e; });

      REQUIRE(result.timers.size() == 3);
      REQUIRE(result.timers[0].counter == 1);
      REQUIRE(result.timers[1].counter == 1);
      REQUIRE(result.timers[2].counter == 4);
   }
#if 0
   SECTION("test if no scope timer was set")
   {
      REQUIRE_THROWS([] { debug_timer::start("main"); }());
   }
#endif
   SECTION("test if scope locking works")
   {
      REQUIRE_THROWS([]() {
         debug_timer::start("main");
         {
            auto v = debug_timer::scope("outer scope");
            {
               auto v1 = debug_timer::scope("scope 1");
            }
            {
               auto v2 = debug_timer::scope("scope 2");
            }
         }
      }());
   }
}
