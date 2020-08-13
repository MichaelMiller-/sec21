#include <catch.hpp>

#include <sec21/row_col.h>

#include <algorithm>
#include <array>
#include <vector>
#include <numeric>

TEST_CASE("row_to_col function", "[sec21][core]")
{
   using namespace sec21;

   SECTION("empty rows")
   {
      const auto input = std::vector<row>{};
      auto result = row_to_col(std::begin(input), std::end(input));
      REQUIRE(std::empty(result));
   }
   SECTION("example row list")
   {
      std::vector<row> input(5);
      std::iota(std::begin(input), std::end(input), row{0});
      auto result = row_to_col(std::begin(input), std::end(input));

      static_assert(std::is_same_v<col, decltype(result)::value_type>);
      const auto expected = std::array{col{0}, col{1}, col{2}, col{3}, col{4}};
      REQUIRE(std::size(result) == std::size(expected));
      REQUIRE(std::equal(std::begin(result), std::end(result), std::begin(expected)));
   }
}
