#include <catch.hpp>

#include <sec21/arity.h>

void func(int, int);

struct binary_functor
{
   template <typename T, typename U>
   void operator()(T, U)
   {
   }
};

TEST_CASE("count the number of arguments", "[sec21][core]")
{
   using namespace sec21;

   SECTION("normal function") { STATIC_REQUIRE(arity(func) == 2); }
   SECTION("generic lambdas")
   {
      const auto l0 = []() {};
      const auto l1 = [](auto) {};
      const auto l2 = [](auto, auto) {};
      const auto l3 = [](auto, auto, auto) {};

      STATIC_REQUIRE(arity(l0) == 0);
      STATIC_REQUIRE(arity(l1) == 1);
      STATIC_REQUIRE(arity(l2) == 2);
      STATIC_REQUIRE(arity(l3) == 3);
   }
   SECTION("generic function object") { STATIC_REQUIRE(arity(binary_functor{}) == 2); }
}
