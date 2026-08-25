#include <catch2/catch_test_macros.hpp>

#include <sec21/meta/sort.h>
#include <sec21/meta/type_list.h>

#include <tuple>
#include <variant>

template <typename Lhs, typename Rhs>
struct predicate : std::bool_constant<sizeof(Lhs) < sizeof(Rhs)>
{
};

TEST_CASE("Sort any given type-list", "[sec21][meta]")
{
   using namespace sec21::meta;

   SECTION("meta::type_list")
   {
      using T1 = type_list<int, float, char, double, short>;
      using R = sort<T1, predicate>::type;

      STATIC_REQUIRE(std::is_same_v<R, type_list<char, short, int, float, double>>);
   }
   SECTION("std::tuple")
   {
      using T1 = std::tuple<int, float, char, double, short>;
      using R = sort<T1, predicate>::type;

      STATIC_REQUIRE(std::is_same_v<R, std::tuple<char, short, int, float, double>>);
   }
   SECTION("std::variant")
   {
      using T1 = std::variant<int, float, char, double, short>;
      using R = sort<T1, predicate>::type;

      STATIC_REQUIRE(std::is_same_v<R, std::variant<char, short, int, float, double>>);
   }
}