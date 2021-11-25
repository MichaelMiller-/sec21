#include <catch.hpp>

#include <vector>
#include <array>

#include <sec21/structural_analysis/node.h>
#include <sec21/units.h>

using namespace sec21::structural_analysis;

TEST_CASE("support mask", "[sec21][structural_analysis][node]") 
{
   using namespace sec21::structural_analysis;
   using namespace sec21::units::literals;

   using node_t = node<2, double>;
   using support_t = node_t::global_support_t;

   std::vector<node_t> input{
      {1u, {0.0, 3.0}},
      {2u, {3.0, 3.0}, support_t{ false, true }},
      {3u, {3.0, 0.0}},
      {4u, {0.0, 0.0}, support_t{ true, true }}
   };

   std::vector<bool> mask;
   support_mask(std::begin(input), std::end(input), std::back_inserter(mask));

   const auto expected = std::array{ false, false, false, true, false, false, true, true };
   REQUIRE(std::equal(std::begin(mask), std::end(mask), std::begin(expected)));
}
TEST_CASE("support mask 2", "[sec21][structural_analysis][node]")
{
   using namespace sec21::structural_analysis;
   using namespace sec21::units::literals;

   using node_t = node<2, double>;
   using support_t = node_t::global_support_t;

   std::vector<node_t> input{
      {1u, {0.0, 3.0}},
      {2u, {3.0, 3.0}},
      {3u, {3.0, 0.0}, support_t{ true, true }},
      {4u, {0.0, 0.0}, support_t{ false, true }}
   };

   std::vector<bool> mask;
   support_mask(std::begin(input), std::end(input), std::back_inserter(mask));

   const auto expected = std::array{ false, false, false, false, true, true, false, true };
   REQUIRE(std::equal(std::begin(mask), std::end(mask), std::begin(expected)));
}