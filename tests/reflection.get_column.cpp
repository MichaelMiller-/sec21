#include <catch2/catch_test_macros.hpp>

#include <sec21/reflection/column.h>
#include <sec21/reflection/get_column.h>
#include <sec21/reflection/table.h>

using namespace sec21;

struct user
{
   std::string name{};
   std::string password{};
   int karma{};
   double cash{};
};

template <>
struct reflection::table<user>
{
   static constexpr auto name = "user";

   struct columns
   {
      using name = reflection::column<"name", &user::name>;
      using password = reflection::column<"password", &user::password>;
      using karma = reflection::column<"karma", &user::karma>;
      using cash = reflection::column<"cash", &user::cash>;
   };

   using metainfo = std::tuple<columns::name, columns::password, columns::karma, columns::cash>;
};

TEST_CASE("helper to get tha value from a column", "[sec21][reflection]")
{
   user input{"John Doe", "secret", 42, 3.14};

   SECTION("getter")
   {
      REQUIRE(reflection::get_column<0>(input) == "John Doe");
      REQUIRE(reflection::get_column<1>(input) == "secret");
      REQUIRE(reflection::get_column<2>(input) == 42);
      REQUIRE(reflection::get_column<3>(input) == 3.14);
   }
   SECTION("setter")
   {
      reflection::get_column<2>(input) = 1;
      REQUIRE(reflection::get_column<2>(input) == 1);
   }
}
