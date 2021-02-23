#include <catch.hpp>

#include <sec21/strong_type.h>
#include <sec21/policy/compare.h>
#include <sec21/policy/print.h>
#include <sec21/policy/increment_alphabet.h>

namespace sec21
{
   using alphabet_t = strong_type<
      std::string, 
      struct alphabet_tag, 
      policy::increment_alphabet, 
      policy::compare,
      policy::print>;
}

TEST_CASE("alphabet strong_type", "[sec21][core]")
{
   using namespace sec21;

   alphabet_t id1{"A1"};
   alphabet_t id2{"Z"};
   alphabet_t invalid_id{};

   SECTION("compare")
   {
      REQUIRE(alphabet_t{ "A" } == alphabet_t{ "A" });
      REQUIRE(alphabet_t{ "A" } != alphabet_t{ "B" });
      REQUIRE(alphabet_t{ "A22" } == alphabet_t{ "A22" });
   }
   SECTION("increment")
   {
      REQUIRE(++alphabet_t{ "A" } == alphabet_t{ "B" });
      REQUIRE(++alphabet_t{ "Z" } == alphabet_t{ "A1" });
      REQUIRE(++alphabet_t{ "F3" } == alphabet_t{ "G3" });
      REQUIRE(++alphabet_t{ "Z42" } == alphabet_t{ "A43" });
   }
}
