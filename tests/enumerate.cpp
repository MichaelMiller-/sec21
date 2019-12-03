#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <vector>
#include <tuple>
#include <array>
#include <string>
#include <string_view>

#include <sec21/enumerate.h>

TEST_CASE("enumerate over array and get current value and index", "[enumerate]")
{
   const std::array<std::string, 3> input{ "foo", "bar", "baz" };

   constexpr std::array<size_t, 3> expected_indicies{ 0, 1, 2 };
   const std::array<std::string, 3> expected_values{ "foo", "bar", "baz" };

#ifdef __cpp_structured_bindings
   for (auto[i, value] : sec21::enumerate(input))
   {
      REQUIRE(i == expected_indicies[i]);
      REQUIRE(value == expected_values[i]);
   }
#endif
}

bool operator == (std::pair<int, std::string> const& lhs, std::pair<int, std::string> const& rhs) noexcept
{
   return lhs.first == rhs.first && lhs.second == rhs.second;
}

TEST_CASE("enumerate over map and get current value and index", "[enumerate]")
{
   const std::map<int, std::string> input{ 
      { 5, "foo" },
      { 6, "bar" },
      { 9, "baz" }
   };

   constexpr std::array<size_t, 3> expected_indicies{ 0, 1, 2 };
   const std::array<std::pair<int, std::string>, 3> expected_values{ {
      { 5, "foo" },
      { 6, "bar" },
      { 9, "baz" }
   } };

#ifdef __cpp_structured_bindings
   for (auto[i, value] : sec21::enumerate(input))
   {
      REQUIRE(i == expected_indicies[i]);
      REQUIRE(value == expected_values[i]);
   }
#endif
}

#ifdef CATCH_CONFIG_ENABLE_BENCHMARKING
TEST_CAST("enumerate vs for_each_indexed", "[benchmark]")
{
   auto i = GENERATE(take(500));

   BENCHMARK("Benchmark: festgehaltene_zeilen mit lookup table als &&")
   {
      auto node_row_lookup = node_matrix_row_lookup_table(std::begin(sys.nodes), std::end(sys.nodes));
      auto result = festgehaltene_zeilen(std::begin(sys.nodes), std::end(sys.nodes), node_row_lookup);
      return result;
   };
   BENCHMARK("Benchmark: festgehaltene_zeilen mit lookup table als const& -> .at() instead []")
   {
      auto node_row_lookup = node_matrix_row_lookup_table(std::begin(sys.nodes), std::end(sys.nodes));
      auto result = v2::festgehaltene_zeilen(std::begin(sys.nodes), std::end(sys.nodes), node_row_lookup);
      return result;
   };
}
#endif
