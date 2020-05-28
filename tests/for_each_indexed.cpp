#include <catch.hpp>

#include <sec21/for_each_indexed.h>

#include <vector>

TEST_CASE("iterate over a sequence", "[sec21][core]")
{
   const auto input = std::vector{ 4, 3, 2, 1 };
   using value_t = decltype(input)::value_type;

   std::vector<std::pair<std::size_t, value_t>> result{};
   sec21::for_each_indexed(
      std::begin(input), 
      std::end(input), 
      [&result](std::size_t index, auto value){ return result.push_back({index, value}); });

   decltype(result) expected{ 
      { 0, 4 },
      { 1, 3 },
      { 2, 2 },
      { 3, 1 }
   };

   REQUIRE(std::size(result) == std::size(expected));
   REQUIRE(std::equal(std::begin(result), std::end(result), std::begin(expected)));
}
