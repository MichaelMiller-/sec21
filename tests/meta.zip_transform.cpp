#include <catch2/catch_test_macros.hpp>

#include <sec21/meta/zip_transform.h>
#include <sec21/meta/type_list.h>

#include <tuple>
#include <variant>

TEST_CASE("Zip and transform two type-lists", "[sec21][meta]")
{
   using namespace sec21::meta;
#if TODO
   SECTION("meta::type_list")
   {
      using T1 = type_list<int>;
      using T2 = type_list<float, char>;
      using R = zip_transform<T1, T2, T3>::type;

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
#endif
}