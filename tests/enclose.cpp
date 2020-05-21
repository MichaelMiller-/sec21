#include <catch.hpp>

#include <sec21/enclose.h>

#include <vector>

TEST_CASE("string format for a example sequence", "[sec21][core]")
{
   const std::vector<int> input{ 1, 2, 3 };

   auto result = sec21::enclose(input);

   const decltype(result) expected{"{1, 2, 3}"};
   REQUIRE(std::size(result) == std::size(expected));
   REQUIRE(std::equal(std::begin(result), std::end(result), std::begin(expected)));
}
TEST_CASE("string format for a sequence with only one element", "[sec21][core]")
{
   const std::vector<int> input{ 1 };

   auto result = sec21::enclose(input);

   const decltype(result) expected{"{1}"};
   REQUIRE(std::size(result) == std::size(expected));
   REQUIRE(std::equal(std::begin(result), std::end(result), std::begin(expected)));
}
TEST_CASE("string format for a empty sequence", "[sec21][core]")
{
   const std::vector<int> input{};

   auto result = sec21::enclose(input);

   const decltype(result) expected{"{}"};
   REQUIRE(std::size(result) == std::size(expected));
   REQUIRE(std::equal(std::begin(result), std::end(result), std::begin(expected)));
}

