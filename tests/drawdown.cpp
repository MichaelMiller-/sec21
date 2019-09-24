//! \todo warnings produced from boost::numeric::ublas
//#define _SILENCE_CXX17_OLD_ALLOCATOR_MEMBERS_DEPRECATION_WARNING
//#define _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
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
      const auto n = std::distance(first, last);
 
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

/*
float MaxDrawdown(const vector<int>& stock)
{
    auto maxs = view::partial_sum(stock, [](auto l, auto r){ return std::max(l, r); });
    auto dds = view::zip_with([](auto peak, auto ith) { return (float)(peak-ith)/peak; }, maxs, stock);
    return 100.0f * ranges::max(dds);
}
*/
      // -> zip | map | reduce

TEST_CASE("add node to system", "[drawdown]")
{
   using namespace sec21;

   const std::vector<int> input{ 15, 22, 22, 23, 21, 25, 28, 24, 23, 22, 12, 16, 20, 30, 35, 29, 25, 21, 22, 26, 28 };

   /* 
   The MDD problem can be formulated as follows: given an array A, find the maximum 
   difference A[j] - A[i] with j < i. The constraint on i and j makes this problem challenging. 
   Without it we can just find the maximum and minimum elements of the array.
   */

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

