#include <catch.hpp>

#include <sec21/structural_analysis/impl/lookup.h>

#include <vector>
#include <algorithm>
#include <array>

TEST_CASE("row_to_col()", "[sec21][structural_analysis_impl][lookup]")
{
   using namespace sec21::structural_analysis::impl;

   SECTION("empty rows")
   {
      const auto input = std::vector<Row>{};
      auto result = row_to_col(std::begin(input), std::end(input));
      REQUIRE(std::empty(result));
   }
   SECTION("example row list")
   {
      std::vector<Row> input(5);
      std::iota(std::begin(input), std::end(input), Row{0});
      auto result = row_to_col(std::begin(input), std::end(input));

      static_assert(std::is_same_v<Col, decltype(result)::value_type>);
      const auto expected = std::array{ Col{0}, Col{1}, Col{2}, Col{3}, Col{4} };
      REQUIRE(std::size(result) == std::size(expected));
      REQUIRE(std::equal(std::begin(result), std::end(result), std::begin(expected)));
   }
}
