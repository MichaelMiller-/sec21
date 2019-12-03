#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <sec21/lookup.h>

namespace ns
{
enum class foo
{
   x = 0,
   y = 1,
   z = 2
};
}

TEST_CASE("lookup", "[core]")
{
   using namespace sec21;
   using namespace ns;

   std::vector<int> unsorted_input_keys = {1, 2, 3, 8, 5};

   lookup<int, ns::foo, int> lookup_table;
   lookup_table.reserve(std::begin(unsorted_input_keys), std::end(unsorted_input_keys));
   {
      std::vector<int> expected = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
      REQUIRE(std::size(expected) == std::distance(lookup_table.raw_values_begin(), lookup_table.raw_values_end()));
      REQUIRE(std::equal(std::begin(expected), std::end(expected), lookup_table.raw_values_begin()));
   }
   // assignment
   lookup_table[2][foo::y] = 43;
   {
      std::vector<int> expected = { 0, 0, 0, 43, 0, 0, 0, 0, 0, 0 };
      REQUIRE(std::size(expected) == std::distance(lookup_table.raw_values_begin(), lookup_table.raw_values_end()));
      REQUIRE(std::equal(std::begin(expected), std::end(expected), lookup_table.raw_values_begin()));
   }
   lookup_table[1][foo::x] = 7;
   {
      std::vector<int> expected = { 7, 0, 0, 43, 0, 0, 0, 0, 0, 0 };
      REQUIRE(std::size(expected) == std::distance(lookup_table.raw_values_begin(), lookup_table.raw_values_end()));
      REQUIRE(std::equal(std::begin(expected), std::end(expected), lookup_table.raw_values_begin()));
   }
   lookup_table[3][foo::x] = 3;
   lookup_table[3][foo::y] = 2;
   {
      std::vector<int> expected = { 7, 0, 0, 43, 3, 2, 0, 0, 0, 0 };
      REQUIRE(std::size(expected) == std::distance(lookup_table.raw_values_begin(), lookup_table.raw_values_end()));
      REQUIRE(std::equal(std::begin(expected), std::end(expected), lookup_table.raw_values_begin()));
   }
   lookup_table[5][foo::x] = 13;
   lookup_table[5][foo::y] = 12;
   {
      std::vector<int> expected = { 7, 0, 0, 43, 3, 2, 0, 0, 13, 12 };
      REQUIRE(std::size(expected) == std::distance(lookup_table.raw_values_begin(), lookup_table.raw_values_end()));
      REQUIRE(std::equal(std::begin(expected), std::end(expected), lookup_table.raw_values_begin()));
   }
   SECTION("access via operator [][]")
   {
      REQUIRE(lookup_table[1][foo::x] == 7);
      REQUIRE(lookup_table[1][foo::y] == 0);
      REQUIRE(lookup_table[2][foo::x] == 0);
      REQUIRE(lookup_table[2][foo::y] == 43);
      REQUIRE(lookup_table[3][foo::x] == 3);
      REQUIRE(lookup_table[3][foo::y] == 2);
      REQUIRE(lookup_table[8][foo::x] == 0);
      REQUIRE(lookup_table[8][foo::y] == 0);
      REQUIRE(lookup_table[5][foo::x] == 13);
      REQUIRE(lookup_table[5][foo::y] == 12);
   }
   SECTION("invalid keys -> exception")
   {
      REQUIRE_THROWS(lookup_table[0][foo::x] == 0);
      REQUIRE_THROWS(lookup_table[12][foo::x] == 43);
   }
   SECTION("clear")
   {
      lookup_table.clear();
      std::vector<int> expected = { };
      REQUIRE(std::size(expected) == std::distance(lookup_table.raw_values_begin(), lookup_table.raw_values_end()));
      REQUIRE(std::equal(std::begin(expected), std::end(expected), lookup_table.raw_values_begin()));
   }
}
