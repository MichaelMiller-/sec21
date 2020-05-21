#include <catch.hpp>

#include <vector>
#include <array>

#include <sec21/structural_analysis/node.h>

using namespace sec21::structural_analysis;

TEST_CASE("support mask", "[sec21][structural_analysis][node]") 
{
   using namespace sec21::structural_analysis;
   using namespace sec21::units::literals;

   using node_t = node<2, double>;
   using support_t = node_t::global_support_t;

   std::vector<node_t> input{
      {1, {0.0, 3.0}},
      {2, {3.0, 3.0}, support_t{ false, true }},
      {3, {3.0, 0.0}},
      {4, {0.0, 0.0}, support_t{ true, true }}
   };

   std::vector<bool> mask;
   support_mask(std::begin(input), std::end(input), std::back_inserter(mask));

   const auto expected = std::array{ false, false, false, true, false, false, true, true };
   REQUIRE(std::equal(std::begin(mask), std::end(mask), std::begin(expected)));
}