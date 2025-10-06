#include <catch2/catch_test_macros.hpp>

#include <sec21/zip.h>

#include <map>
#include <vector>

using namespace sec21;

TEST_CASE("zip two containers to a std::map", "[sec21]")
{
   const std::vector keys{1, 2, 3};
   const std::vector<std::string> values{"foo", "bar", "baz"};

   std::map<int, std::string> result;

   zip([&result](auto key, auto value) { result[key] = value; }, begin(keys), end(keys), begin(values));

   const decltype(result) expected{
      {1, "foo"},
      {2, "bar"},
      {3, "baz"},
   };

   REQUIRE(size(result) == size(expected));
   REQUIRE(std::equal(begin(result), end(result), begin(expected)));
}

TEST_CASE("zip three sequences into one", "[sec21]")
{
   const std::vector input1{1, 4, 7};
   const std::vector input2{2, 5, 8};
   const std::vector input3{3, 6, 9};

   std::vector<int> result;

   zip(
      [&result](auto v1, auto v2, auto v3) {
         result.push_back(v1);
         result.push_back(v2);
         result.push_back(v3);
      },
      begin(input1), end(input1), begin(input2), begin(input3));

   const decltype(result) expected{1, 2, 3, 4, 5, 6, 7, 8, 9};

   REQUIRE(size(result) == size(expected));
   REQUIRE(std::equal(begin(result), end(result), begin(expected)));
}

TEST_CASE("zip two unequal sequences where the second sequence is longer", "[sec21]")
{
   const std::vector input1{1, 4, 7};
   const std::vector input2{1, 2, 3, 4, 5};

   std::vector<int> result;

   zip(
      [&result](auto v1, auto v2) {
         result.push_back(v1);
         result.push_back(v2);
      },
      begin(input1), end(input1), begin(input2));

   const decltype(result) expected{1, 1, 4, 2, 7, 3};

   REQUIRE(size(result) == size(expected));
   REQUIRE(std::equal(begin(result), end(result), begin(expected)));
}
