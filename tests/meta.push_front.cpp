#include <catch2/catch_test_macros.hpp>

#include <sec21/meta/push_front.h>
#include <sec21/meta/type_list.h>

TEST_CASE("Push a given type to the front of any list", "[sec21][meta]")
{
   using namespace sec21::meta;

   SECTION("meta::type_list")
   {
      using input_t = type_list<int, float>;
      using output_t = push_front_t<char, input_t>;

      STATIC_REQUIRE(std::is_same_v<output_t, type_list<char, int, float>>);
   }
   SECTION("std::tuple")
   {
      using input_t = std::tuple<int, float>;
      using output_t = push_front_t<char, input_t>;

      STATIC_REQUIRE(std::is_same_v<output_t, std::tuple<char, int, float>>);
   }
}