#include <catch2/catch_test_macros.hpp>

#include <sec21/any_of.h>

TEST_CASE("zero overhead syntax helper any_of()", "[sec21]")
{
   constexpr int x{10};
   constexpr int y{50};
   constexpr int z{99};

   STATIC_REQUIRE(sec21::any_of{x, y, z} < 100);
   STATIC_REQUIRE(sec21::any_of{x, y, z} <= 99);
   STATIC_REQUIRE(sec21::any_of{x, y, z} < 50);
   STATIC_REQUIRE(sec21::any_of{x, y, z} > 2);
   STATIC_REQUIRE(sec21::any_of{x, y, z} > 9);
   STATIC_REQUIRE(sec21::any_of{x, y, z} >= 10);

   STATIC_REQUIRE((sec21::any_of{x, y, z} < 10) == false);
   STATIC_REQUIRE((sec21::any_of{x, y, z} < 9) == false);
   STATIC_REQUIRE((sec21::any_of{x, y, z} < 1) == false);

   constexpr auto lambda1 = [](auto e) { return e == 100; };
   constexpr auto lambda2 = [](auto e) { return e == 5; };
   constexpr auto lambda3 = [](auto e) { return e == 10; };

   auto f1 = sec21::any_of{lambda1, lambda2, lambda3};

   REQUIRE(f1(6) == false);
   REQUIRE(f1(5) == true);
   REQUIRE(f1(10) == true);
   REQUIRE(f1(15) == false);
   REQUIRE(f1(100) == true);
   REQUIRE(f1(116) == false);
}
