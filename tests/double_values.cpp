#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <sec21/double_values.h>

#include <tuple>

struct foo
{
   std::string s{};
   int i{};
   double d{};

   friend bool operator == (foo const& lhs, foo const& rhs) noexcept {
      return lhs.i == rhs.i && lhs.d == rhs.d && lhs.s == rhs.s;
   }
};

TEST_CASE("double_values", "[core]")
{
   using namespace sec21;

   SECTION("simple test")
   {
      std::vector<int> input{ 1, 2, 3 };
      auto result = double_values<4>(input.begin(), input.end());
      const decltype(result) expected{ 1,1,1,1,2,2,2,2,3,3,3,3 };
      REQUIRE(result == expected);
   }
   SECTION("transformation")
   {
      std::vector<int> input{ 1, 2, 3 };
      auto result = double_values<2, std::string>(input.begin(), input.end(), [](auto && e){ return std::to_string(e); });
      const decltype(result) expected{ "1","1","2","2","3","3" };
      REQUIRE(result == expected);
   }
   SECTION("transformation into struct")
   {
      std::vector<int> input{ 1, 2, 3 };

      auto result = double_values<2, foo>(input.begin(), input.end(), [](auto && e){ return foo{ std::to_string(e), e, 3.14}; });
      const decltype(result) expected{ 
         { "1", 1, 3.14 },
         { "1", 1, 3.14 },
         { "2", 2, 3.14 },
         { "2", 2, 3.14 },
         { "3", 3, 3.14 },
         { "3", 3, 3.14 } };
      REQUIRE(result == expected);
   }
   SECTION("transformation into tuple")
   {
      std::vector<int> input{ 1, 2, 3 };

      using T = std::tuple<std::string, int, double>;
      auto result = double_values<2, T>(input.begin(), input.end(), [](auto && e){ return std::tuple{ std::to_string(e), e, 3.14}; });
      const decltype(result) expected{ 
         { "1", 1, 3.14 },
         { "1", 1, 3.14 },
         { "2", 2, 3.14 },
         { "2", 2, 3.14 },
         { "3", 3, 3.14 },
         { "3", 3, 3.14 } };
      REQUIRE(result == expected);
   }
}

