#include <catch2/catch_test_macros.hpp>

#include <sec21/meta/merge.h>
#include <sec21/meta/type_list.h>

#include <tuple>
#include <variant>

TEST_CASE("Merge several type-list into one list", "[sec21][meta]")
{
   using namespace sec21::meta;

   SECTION("meta::type_list")
   {
      using T1 = type_list<int>;
      using T2 = type_list<float>;
      using T3 = type_list<char>;
      using R = merge<T1, T2, T3>::type;

      STATIC_REQUIRE(std::is_same_v<R, type_list<int, float, char>>);
   }
   SECTION("std::tuple")
   {
      using T1 = std::tuple<int>;
      using T2 = std::tuple<float>;
      using T3 = std::tuple<char>;
      using R = merge<T1, T2, T3>::type;

      STATIC_REQUIRE(std::is_same_v<R, std::tuple<int, float, char>>);
   }
   SECTION("std::variant")
   {
      using T1 = std::variant<int>;
      using T2 = std::variant<float>;
      using T3 = std::variant<char>;
      using R = merge<T1, T2, T3>::type;

      STATIC_REQUIRE(std::is_same_v<R, std::variant<int, float, char>>);
   }
}