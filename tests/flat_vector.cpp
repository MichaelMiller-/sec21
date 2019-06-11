#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <sec21/flat_vector.h>

TEST_CASE("flatten any sequence to a vector of T", "[core]")
{
   using namespace sec21;

   SECTION("empty sequence")
   {
      std::vector<int> input{ };
      auto result = flat_vector<int>(std::begin(input), std::end(input));

      REQUIRE(std::size(result) == 0);
   }
   SECTION("1D sequence")
   {
      std::vector<int> input{ 1, 2, 3, 4, 67, 8 };
      auto result = flat_vector<int>(std::begin(input), std::end(input));

      const std::vector<int> expected{ 1, 2, 3, 4, 67, 8 };
      REQUIRE(result == expected);
   }
   SECTION("vector of vector input sequence")
   {
      std::vector input{ 
         std::vector{ 1, 2, 3 },
         std::vector{ 4, 5 },
         std::vector{ 7, 8, 9 }
      };
      auto result = flat_vector<int>(std::begin(input), std::end(input));

      const std::vector<int> expected{ 1, 2, 3, 4, 5, 7, 8, 9 };
      REQUIRE(result == expected);
   }
}
