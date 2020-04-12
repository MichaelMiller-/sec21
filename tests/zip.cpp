#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <sec21/zip.h>

#include <vector>

TEST_CASE("zip two containers to a map", "[sec21][core]")
{
   using namespace sec21;

   const std::vector<int> keys{ 1, 2, 3 };
   const std::vector<std::string> values{ "foo", "bar", "baz" };

   std::map<int, std::string> result;

   sec21::zip(
      [&result](auto&& tuple) { result[std::get<0>(tuple)] = std::get<1>(tuple); }, 
      std::begin(keys), 
      std::end(keys), 
      std::begin(values));   

   const decltype(result) expected{
      { 1, "foo" },
      { 2, "bar" },
      { 3, "baz" },
   };

   REQUIRE(std::size(result) == std::size(expected));
   REQUIRE(std::equal(std::begin(result), std::end(result), std::begin(expected)));
}

TEST_CASE("zip three sequences to one", "[sec21][core]")
{
   const std::vector<int> input1{ 1, 4, 7 };
   const std::vector<int> input2{ 2, 5, 8 };
   const std::vector<int> input3{ 3, 6, 9 };

   std::vector<int> result;

   sec21::zip(
      [&result](auto&& tuple) { 
         result.push_back(std::get<0>(tuple));
         result.push_back(std::get<1>(tuple));
         result.push_back(std::get<2>(tuple));
      },
      std::begin(input1), 
      std::end(input1), 
      std::begin(input2),
      std::begin(input3));   

   const decltype(result) expected{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };

   REQUIRE(std::size(result) == std::size(expected));
   REQUIRE(std::equal(std::begin(result), std::end(result), std::begin(expected)));   
}