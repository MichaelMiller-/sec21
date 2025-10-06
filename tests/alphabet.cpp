#include <catch2/catch_test_macros.hpp>

#include <sec21/alphabet.h>

TEST_CASE("alphabet strong_type", "[sec21]")
{
   using namespace sec21;

   alphabet_t id1{"A1"};
   alphabet_t id2{"Z"};
   alphabet_t invalid_id{};

   SECTION("compare")
   {
      REQUIRE(alphabet_t{"A"} == alphabet_t{"A"});
      REQUIRE(alphabet_t{"A"} != alphabet_t{"B"});
      REQUIRE(alphabet_t{"A22"} == alphabet_t{"A22"});
   }
   SECTION("increment")
   {
      REQUIRE(++alphabet_t{"A"} == alphabet_t{"B"});
      REQUIRE(++alphabet_t{"Z"} == alphabet_t{"A1"});
      REQUIRE(++alphabet_t{"F3"} == alphabet_t{"G3"});
      REQUIRE(++alphabet_t{"Z42"} == alphabet_t{"A43"});
   }
}
