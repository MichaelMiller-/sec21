#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include "../command_loadfile.h"

TEST_CASE("validate input action: loadfile", "[sec21][editor.backend]")
{
   const auto input = R"(
   {
      "command": "loadfile",
      "argument": "filename"
   }
   )"_json;

   using namespace sec21;

   validate<editor::command_loadfile_t> obj;
   REQUIRE(obj("anything").has_value() == false);
   REQUIRE(obj(input.dump()).has_value());
}
