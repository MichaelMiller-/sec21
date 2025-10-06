#include <catch2/catch_test_macros.hpp>

#include <sec21/reflection/column.h>
#include <sec21/write_csv.h>

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

TEST_CASE("transform a range", "[sec21]")
{
   std::vector<user> input{{"John Doe", "secret", 42, 0}, {"Max Mustermann", "****", 1, 45'678}};

   std::stringstream stream;
   write_csv(stream, input);

   const auto expected = R"(name, password, karma, cash
John Doe, secret, 42, 0
Max Mustermann, ****, 1, 45678
)";

   REQUIRE(stream.str() == expected);
}
