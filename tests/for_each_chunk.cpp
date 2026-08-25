#include <catch2/catch_test_macros.hpp>

#include <sec21/for_each_chunk.h>

#include <vector>

TEST_CASE("apply a function to every chunk in a sequence", "[sec21]")
{
   using namespace sec21;

   SECTION("copy values into a new range")
   {
      const auto input = std::vector{1, 2, 3, 4, 5, 6, 7, 8};
      using value_t = decltype(input)::value_type;

      std::vector<std::pair<value_t, value_t>> result{};
      for_each_chunk<2>(begin(input), end(input),
                        [&result](auto value1, auto value2) { result.push_back({value1, value2}); });

      const auto expected = decltype(result){{1, 2}, {3, 4}, {5, 6}, {7, 8}};
      REQUIRE(std::size(result) == std::size(expected));
      REQUIRE(std::equal(std::begin(result), std::end(result), std::begin(expected)));
   }
   SECTION("test with an empty input range")
   {
      const auto input = std::vector<int>{};
      using value_t = decltype(input)::value_type;

      std::vector<std::pair<value_t, value_t>> result{};
      for_each_chunk<2>(input, [&result](auto x, auto y) { result.push_back(std::make_pair(x, y)); });

      REQUIRE(std::size(result) == 0);
   }
   SECTION("mutate the input range")
   {
      auto result = std::vector{1, 2, 3, 4, 5, 6, 7, 8};

      for_each_chunk<2>(result, [](auto& x, auto& y) { x = y; });

      const auto expected = decltype(result){2, 2, 4, 4, 6, 6, 8, 8};
      REQUIRE(std::size(result) == std::size(expected));
      REQUIRE(std::equal(std::begin(result), std::end(result), std::begin(expected)));
   }
   SECTION("input range cannot be divided by chunk-size")
   {
      REQUIRE_THROWS([] {
         const auto input = std::vector{1, 2, 3, 4, 5, 6, 7, 8};
         for_each_chunk<3>(input, [](auto, auto, auto) { /* nothing */ });
      }());
   }
}
