#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include "../command_add_support.h"

TEST_CASE("validate input action: add support", "[sec21][editor.backend]")
{
   const auto input = R"(
   {
      "command": "add_support",
      "argument": {
         "node": 1,
         "px": true,
         "py": false
      }
   }
   )"_json;

   using namespace sec21;

   validate<editor::command_add_support_t> obj;
   REQUIRE(obj("anything").has_value() == false);
   REQUIRE(obj(input.dump()).has_value());
}
