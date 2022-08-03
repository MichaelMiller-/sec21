#include <catch.hpp>

#include <sec21/type_traits/has_type.h>

TEST_CASE("checks whether an explicit type is in a type list", "[sec21][type_traits]")
{
   using namespace sec21::type_traits;

   SECTION("variadic template list")
   {
      STATIC_REQUIRE(has_type<int, int, float, char>::value == true);
      STATIC_REQUIRE(has_type<float, int, float, char>::value == true);
      STATIC_REQUIRE(has_type<char, int, float, char>::value == true);
      STATIC_REQUIRE(has_type<double, int, float, char>::value == false);
   }
   SECTION("std::tuple")
   {
      using test_tuple_t = std::tuple<int, float, double>;

      STATIC_REQUIRE(has_type<int, test_tuple_t>::value == true);
      STATIC_REQUIRE(has_type<float, test_tuple_t>::value == true);
      STATIC_REQUIRE(has_type<double, test_tuple_t>::value == true);
      STATIC_REQUIRE(has_type<char, test_tuple_t>::value == false);
   }
   SECTION("std::variant")
   {
      using test_variant_t = std::variant<int, float, double>;

      STATIC_REQUIRE(has_type<int, test_variant_t>::value == true);
      STATIC_REQUIRE(has_type<float, test_variant_t>::value == true);
      STATIC_REQUIRE(has_type<double, test_variant_t>::value == true);
      STATIC_REQUIRE(has_type<char, test_variant_t>::value == false);
   }
}