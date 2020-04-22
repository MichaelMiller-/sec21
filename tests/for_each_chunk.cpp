#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <sec21/for_each_chunk.h>

#include <vector>
#include <array>

TEST_CASE("apply a function to every chunk in a sequence", "[sec21][core]")
{
   const auto input = std::vector{ 1, 2, 3, 4, 5, 6, 7, 8 };
   using value_t = decltype(input)::value_type;

   std::vector<std::pair<value_t, value_t>> result{};
   sec21::for_each_chunk<2>(
      std::begin(input), 
      std::end(input), 
      [&result](auto value1, auto value2){ return result.push_back({value1, value2}); });


   const auto expected = std::vector<std::pair<value_t, value_t>>{ 
      { 1, 2 },
      { 3, 4 },
      { 5, 6 },
      { 7, 8 }
   };

   REQUIRE(std::size(result) == std::size(expected));
   REQUIRE(std::equal(std::begin(result), std::end(result), std::begin(expected)));
}
