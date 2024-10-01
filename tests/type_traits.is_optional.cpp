#include <catch.hpp>

#include <sec21/type_traits/is_optional.h>

TEST_CASE("checks whether the type is a std::optional", "[sec21][type_traits]")
{
   using namespace sec21::type_traits;

   STATIC_REQUIRE(is_optional_v<std::optional<int>> == true);
   STATIC_REQUIRE(is_optional_v<int> == false);
}
