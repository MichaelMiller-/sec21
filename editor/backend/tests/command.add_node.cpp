#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include "../command_add_node.h"

TEST_CASE("validate input action: add node", "[sec21][editor.backend]")
{
   const auto input = R"(
   {
      "command": "add_node",
      "argument": {
         "x": 3.14,
         "y": 1.23
      }
   }
   )"_json;

   using namespace sec21;

   validate<editor::command_add_node_t> obj;
   REQUIRE(obj("anything").has_value() == false);
   REQUIRE(obj(input.dump()).has_value());
}
