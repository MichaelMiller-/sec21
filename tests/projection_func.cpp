#include <catch.hpp>

#include <algorithm>
#include <vector>

#include <sec21/projection_func.h>

TEST_CASE("projection wrapper function", "[sec21][core]")
{
   using namespace sec21;

   struct foo
   {
      int a;
      double b;
   };

   std::vector<foo> input{{1, 3.12}, {4, 3.12}, {5, 76.7}, {3, 7.2}, {2, 0.82}};

   SECTION("sort example")
   {
      std::sort(std::begin(input), std::end(input), projection_func{std::less{}, &foo::a});

      decltype(input) const expected{{1, 3.12}, {2, 0.82}, {3, 7.2}, {4, 3.12}, {5, 76.7}};

      REQUIRE(std::equal(std::begin(input), std::end(input), std::begin(expected), std::end(expected),
                         // only member foo::a is needed for this test
                         [](auto const& lhs, auto const& rhs) { return lhs.a == rhs.a; }) == true);
   }
   SECTION("find example")
   {
      auto const it = std::find_if(std::begin(input), std::end(input), projection{std::equal_to{}, &foo::a, 5});

      REQUIRE(it != std::end(input));
      REQUIRE(it->a == 5);
      REQUIRE(it->b == Catch::Approx(76.7));
   }
}