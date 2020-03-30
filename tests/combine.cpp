#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <sec21/combine.h>

TEST_CASE("combine lambdas; all_of", "[sec21][core]") 
{
   constexpr auto lambda1 = [](auto e){ return e < 100; };
   constexpr auto lambda2 = [](auto e){ return e > 5; };
   constexpr auto lambda3 = [](auto e){ return e > 10; };

   auto f1 = sec21::all_of(lambda1, lambda2, lambda3);

   STATIC_REQUIRE(f1(6) == false);
   STATIC_REQUIRE(f1(16) == true);
   STATIC_REQUIRE(f1(116) == false);
}

TEST_CASE("combine lambdas; any_of", "[sec21][core]") 
{
   constexpr auto lambda1 = [](auto e){ return e == 100; };
   constexpr auto lambda2 = [](auto e){ return e == 5; };
   constexpr auto lambda3 = [](auto e){ return e == 10; };

   auto f1 = sec21::any_of(lambda1, lambda2, lambda3);

   STATIC_REQUIRE(f1(6) == false);
   STATIC_REQUIRE(f1(5) == true);
   STATIC_REQUIRE(f1(10) == true);
   STATIC_REQUIRE(f1(15) == false);
   STATIC_REQUIRE(f1(100) == true);
   STATIC_REQUIRE(f1(116) == false);
}
