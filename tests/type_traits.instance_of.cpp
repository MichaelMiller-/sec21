#include <catch2/catch_test_macros.hpp>

#include <sec21/type_traits/is_instance_of.h>

TEST_CASE("Checks whether a type corresponds to an instantiation of a class", "[sec21][type_traits]")
{
   using namespace sec21::type_traits;

   STATIC_REQUIRE(is_instance_of_v<std::basic_string, std::string> == true);
   STATIC_REQUIRE(is_instance_of_v<std::tuple, std::tuple<int, double>> == true);
   STATIC_REQUIRE(is_instance_of_v<std::tuple, std::vector<int>> == false);
   STATIC_REQUIRE(is_instance_of_v<std::vector, std::tuple<int, double>> == false);
}
