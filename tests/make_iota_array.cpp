#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <sec21/make_iota_array.h>

#include <algorithm>

#ifdef __cpp_lib_constexpr_misc

TEST_CASE("make iota array", "[sec21][core]") 
{
   constexpr auto result = make_iota_array<int, 4>();
   const decltype(result) expected{ 0,1,2,3 };

   STATIC_REQUIRE(std::equal(std::begin(result), std::end(result), std::begin(expected)), "sould be equal");
}

#else

TEST_CASE("make iota array", "[sec21][core]")
{
   using namespace sec21;

   SECTION("simple test")
   {
      auto result = make_iota_array<int, 4>();
      const decltype(result) expected{ 0,1,2,3 };
      REQUIRE(std::equal(std::begin(result), std::end(result), std::begin(expected)) == true);
   }
   SECTION("with offset")
   {
      constexpr auto result = make_iota_array<int, 4>(2);
      const decltype(result) expected{ 2,3,4,5 };
      REQUIRE(std::equal(std::begin(result), std::end(result), std::begin(expected)) == true);
   }
}   
#endif