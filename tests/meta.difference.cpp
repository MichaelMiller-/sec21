#include <catch2/catch_test_macros.hpp>

#include <sec21/meta/difference.h>
#include <sec21/meta/type_list.h>

TEST_CASE("Return the difference of two type-lists", "[sec21][meta]")
{
   using namespace sec21::meta;

   SECTION("meta::type_list")
   {
      using T1 = type_list<int, float, char>;
      using T2 = type_list<double, float, short, int>;

      using R1 = difference<T1, T2>::type;
      STATIC_REQUIRE(std::is_same_v<R1, type_list<char>>);

      using R2 = difference<T2, T1>::type;
      STATIC_REQUIRE(std::is_same_v<R2, type_list<double, short>>);
   }
   SECTION("std::tuple")
   {
      using T1 = std::tuple<int, char>;
      using T2 = std::tuple<double, float, short, int>;

      using R1 = difference<T1, T2>::type;
      STATIC_REQUIRE(std::is_same_v<R1, std::tuple<char>>);
   }
}