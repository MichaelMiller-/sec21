#include <catch2/catch_test_macros.hpp>

#include <sec21/meta/remove_if.h>
#include <sec21/meta/type_list.h>

#include <tuple>
#include <variant>

template <typename T>
struct predicate : std::bool_constant<sizeof(T) == 4>
{
};

TEST_CASE("Remove a type that satisfy a predicate from a type-list", "[sec21][meta]")
{
   using namespace sec21::meta;

   SECTION("meta::type_list")
   {
      using T1 = type_list<int, float, char>;
      using R = remove_if<T1, predicate>::type;

      STATIC_REQUIRE(std::is_same_v<R, type_list<char>>);
   }
   SECTION("std::tuple")
   {
      using T1 = std::tuple<int, float, char>;
      using R = remove_if<T1, predicate>::type;

      STATIC_REQUIRE(std::is_same_v<R, std::tuple<char>>);
   }
   SECTION("std::variant")
   {
      using T1 = std::variant<int, float, char>;
      using R = remove_if<T1, predicate>::type;

      STATIC_REQUIRE(std::is_same_v<R, std::variant<char>>);
   }
}