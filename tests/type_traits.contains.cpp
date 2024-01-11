#include <catch.hpp>

#include <sec21/type_traits/contains.h>

TEST_CASE("Check whether an explicit type is in a type list", "[sec21][type_traits]")
{
   using namespace sec21::type_traits;

   SECTION("std::tuple")
   {
      using test_tuple_t = std::tuple<int, float, char>;

      STATIC_REQUIRE(contains<int, test_tuple_t>::value == true);
      STATIC_REQUIRE(contains<std::string, test_tuple_t>::value == false);
      STATIC_REQUIRE(contains<float, test_tuple_t>::value == true);
      STATIC_REQUIRE(contains<double, test_tuple_t>::value == false);
      STATIC_REQUIRE(contains<char, test_tuple_t>::value == true);
   }
   SECTION("std::variant")
   {
      using test_variant_t = std::variant<int, float, char>;

      STATIC_REQUIRE(contains<int, test_variant_t>::value == true);
      STATIC_REQUIRE(contains<float, test_variant_t>::value == true);
      STATIC_REQUIRE(contains<char, test_variant_t>::value == true);
      STATIC_REQUIRE(contains_v<double, test_variant_t> == false);
      STATIC_REQUIRE(contains<std::string, test_variant_t>::value == false);
      STATIC_REQUIRE(contains<std::tuple<>, test_variant_t>::value == false);
   }
   SECTION("Variadic list")
   {
      STATIC_REQUIRE(contains_v<int, int, float, char> == true);
      STATIC_REQUIRE(contains_v<float, int, float, char> == true);
      STATIC_REQUIRE(contains<char, int, float, char>::value == true);
      STATIC_REQUIRE(contains_v<double, int, float, char> == false);
      STATIC_REQUIRE(contains<std::string, int, float, char>::value == false);
   }
}
