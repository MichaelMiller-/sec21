#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <sec21/numeric/make_vector.h>
#include <sec21/units.h>

TEST_CASE("create a vector from a std-sequence (vector)", "[sec21][numeric]") 
{
   using namespace sec21::numeric;

   const auto input = std::vector{ 1, 3, 4, 6, 8, 9, 10, 11, 2 };
   auto result = make_vector<int>(std::begin(input), std::end(input));

   REQUIRE(std::size(result) == std::size(input));
   REQUIRE(std::equal(std::begin(result), std::end(result), std::begin(input), std::end(input)));
}

TEST_CASE("create a vector from a sequence with units", "[sec21][numeric]") 
{
   using namespace sec21::numeric;
   using value_t = sec21::units::quantity<sec21::units::newton, int>;

   const auto input = std::vector<value_t>{ { 1 }, { 2 }, { 422 }, { 17 } };
   auto result = make_vector(std::begin(input), std::end(input), [](auto&& q){ return q.value(); });

   const auto expected = std::vector{ 1, 2, 422, 17 };
   REQUIRE(std::size(result) == std::size(expected));
   REQUIRE(std::equal(std::begin(result), std::end(result), std::begin(expected), std::end(expected)));
}
