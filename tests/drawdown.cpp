#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include "approx_equal.h"

#include <vector>
#include <algorithm>
#include <iterator>
#include <numeric>

namespace sec21
{
   // drawdowns are the differences between the current value and the value at the previous maximum peak.
   template <typename Iterator>
   auto max_drawdown(Iterator first, Iterator last)
   {
      using value_t = typename std::iterator_traits<Iterator>::value_type;
      const size_t n = std::distance(first, last);
 
      std::vector<value_t> maxs(n);
      std::partial_sum(first, last, std::begin(maxs), [](auto l, auto r) { return std::max(l, r); });

      return std::inner_product(
         std::begin(maxs), 
         std::end(maxs), 
         first, 
         0, 
         [](auto l, auto r) { return std::max(l, r); }, 
         std::minus<>{});
   }
}

TEST_CASE("add node to system", "[sec21][core]")
{
   using namespace sec21;

   const std::vector<int> input{ 15, 22, 22, 23, 21, 25, 28, 24, 23, 22, 12, 16, 20, 30, 35, 29, 25, 21, 22, 26, 28 };

   SECTION("example range 1")
   {
      auto it = std::begin(input);
      std::advance(it, 11);
      REQUIRE(max_drawdown(std::begin(input), it) == 16);
   }
   SECTION("example range 2")
   {
      auto it = std::begin(input);
      std::advance(it, 12);
      REQUIRE(max_drawdown(it, std::end(input)) == 14);
   }
}

