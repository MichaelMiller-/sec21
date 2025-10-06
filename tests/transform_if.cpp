#include <catch2/catch_test_macros.hpp>

#include <sec21/transform_if.h>

#include <vector>

struct Data
{
   int id{};

   friend bool operator==(Data const&, Data const&) = default;
};

TEST_CASE("transform a range", "[sec21]")
{
   std::vector in{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
   std::vector<Data> result{};
   // clang-format off
   sec21::transform_if(
      begin(in),
      end(in),
      back_inserter(result),
      [](auto i) { return i < 5; },
      [](auto i) { return Data{i}; });
   // clang-format on

   const auto expected = std::vector<Data>{Data{1}, Data{2}, Data{3}, Data{4}};
   REQUIRE(std::equal(begin(result), end(result), begin(expected)));
}
