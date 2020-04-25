#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <sec21/erase_if_index.h>

#include <vector>
#include <array>

TEST_CASE("erase a element from a sequence if predicate, with index as argument, is true", "[sec21][core]")
{
   using namespace sec21;

   const auto input = std::vector{ 1, 2, 3, 5, 6, 7, 8 };

   auto result = erase_if_index(input, [](auto i){ return i == 2 || i == 4; });

   const auto expected = std::array{ 1, 2, 5, 7, 8 };

   REQUIRE(std::size(result) == std::size(expected));
   REQUIRE(std::equal(std::begin(result), std::end(result), std::begin(expected)));   
}
