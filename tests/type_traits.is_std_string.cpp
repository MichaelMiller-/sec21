#include <catch2/catch_test_macros.hpp>

#include <sec21/type_traits/is_std_string.h>

TEST_CASE("checks whether the type is a std::string", "[sec21][type_traits]")
{
   using namespace sec21::type_traits;

   STATIC_REQUIRE(is_std_string<std::string>::value);
   STATIC_REQUIRE(is_std_string<int>::value == false);
   STATIC_REQUIRE(is_std_string<std::wstring>::value == false);
   STATIC_REQUIRE(is_std_string<float>::value == false);

   STATIC_REQUIRE(is_std_string_v<std::string>);
   STATIC_REQUIRE(is_std_string_v<float> == false);
   STATIC_REQUIRE(is_std_string_v<int> == false);
}