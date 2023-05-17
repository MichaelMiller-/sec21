#include <catch.hpp>

#include <sec21/for_each_adjacent.h>

#include <vector>

TEST_CASE("apply a function for two consecutive elements in a sequence", "[sec21][core]")
{
   using namespace sec21;

   SECTION("copy values into a new range")
   {
      const auto input = std::vector{ 1, 2, 3, 4, 5, 6, 7, 8 };
      using value_t = decltype(input)::value_type;

      std::vector<std::pair<value_t, value_t>> result{};
      for_each_adjacent(
         begin(input),
         end(input),
         [&result](auto value1, auto value2){ result.push_back({value1, value2}); });

      const auto expected = decltype(result){
         { 1, 2 },
         { 2, 3 },
         { 3, 4 },
         { 4, 5 },
         { 5, 6 },
         { 6, 7 },
         { 7, 8 },
      };
      REQUIRE(std::size(result) == std::size(expected));
      REQUIRE(std::equal(std::begin(result), std::end(result), std::begin(expected)));
   }
}
