#include <catch.hpp>

#include <sec21/compose.h>

int f(int i) { return i + 1; }
int g(int i) { return i * i; }

std::optional<int> f_optional(int i) { return i + 1; }
std::optional<int> g_optional(int i) { return i * i; }

TEST_CASE("compose callable objects together", "[sec21]")
{
   using namespace sec21;

   SECTION("compose functions")
   {
      auto fg = compose(f, g);
      auto result = fg(3);
      REQUIRE(result == 10);
   }
   SECTION("compose lamdas")
   {
      auto fg = compose([](auto i) { return i + 1; }, [](auto i) { return i * i; });
      auto result = fg(3);
      REQUIRE(result == 10);
   }
   SECTION("compose functions with optional return types")
   {
      auto fg = compose(f_optional, g_optional);
      auto result = fg(3);
      REQUIRE(result.has_value());
      REQUIRE(result == 10);
   }
}