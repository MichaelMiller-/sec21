#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include "sec21/ranges.h"

#include <algorithm>
#include <iostream>

TEST_CASE("intersection test", "[ranges]")
{
   using namespace sec21;
   using namespace std;

   const vector<int> a = { 10, 2, 4, 5, 6, 7, 8, 9 };
   const vector<int> b = {    2, 3,          7,    9, 10 };
   
   auto c = a | intersect(b);

   const vector<int> expected = { 10, 2, 7, 9 };

   REQUIRE(equal(begin(c), end(c), begin(expected)));
}

TEST_CASE("difference test", "[ranges]")
{
   using namespace sec21;
   using namespace std;

   const vector<int> a = { 1, 2, 4, 5, 6, 7, 8, 9, 10 };
   const vector<int> b = { 2, 3,          7,    9, 10 };

   auto c = a | difference(b);

   const vector<int> expected = { 1, 4, 5, 6, 8 };

   REQUIRE(equal(begin(c), end(c), begin(expected)));
}