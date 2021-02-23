#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include "../command_add_member.h"

TEST_CASE("validate input action: add member", "[sec21][editor.backend]")
{
   const auto input = R"(
   {
      "command": "add_member",
      "argument": {
         "from": 1,
         "to": 2
      }
   }
   )"_json;

   using namespace sec21;

   validate<editor::command_add_member_t> obj;
   REQUIRE(obj("anything").has_value() == false);
   REQUIRE(obj(input.dump()).has_value());
}
