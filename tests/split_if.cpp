#include <catch.hpp>

#include <sec21/split_if.h>

#include <vector>

TEST_CASE("split a container ", "[sec21][core]") {

   std::vector in{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
   std::vector<int> result1{};
   std::vector<int> result2{};

   sec21::split_if(begin(in), end(in), back_inserter(result1), back_inserter(result2), [](auto i) { return i < 5; });

   const auto expected1 = std::vector{ 1, 2, 3, 4 };
   const auto expected2 = std::vector{ 5, 6, 7, 8, 9, 10 };

   REQUIRE(std::equal(begin(result1), end(result1), begin(expected1)));
   REQUIRE(std::equal(begin(result2), end(result2), begin(expected2)));
}
