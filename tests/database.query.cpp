#include <catch.hpp>

#include <sec21/database/constraints.h>
#include <sec21/database/query.h>
#include <sec21/reflection/column.h>
#include <sec21/reflection/table.h>

#include <tuple>

struct user
{
   std::string name{};
   std::string password{};
   int karma{};
   double cash{};
};

namespace sec21::reflection
{
   template <>
   struct table<user>
   {
      static constexpr auto name = "user";

      struct columns
      {
         using name = column<"name", &user::name, database::primary_key, database::not_null>;
         using password = column<"password", &user::password, database::not_null>;
         using karma = column<"karma", &user::karma>;
         using cash = column<"cash", &user::cash>;
      };

      using metainfo = std::tuple<columns::name, columns::password, columns::karma, columns::cash>;
   };
} // namespace sec21::reflection

TEST_CASE("test helper implementation", "[sec21][database]")
{
#if __cpp_designated_initializers >= 201707L
   user obj{.name = "John Doe", .password = "hidden", .karma = 42, .cash = 3.14};
#else
   user obj{"John Doe", "hidden", 42, 3.14};
#endif

   using namespace sec21;
   using reflection_t = reflection::table<decltype(obj)>::metainfo;

   SECTION("concat column names")
   {
      REQUIRE(database::detail::concat_column_names<reflection_t>() == "name,password,karma,cash");
   }
   SECTION("embrace concat column names")
   {
      REQUIRE(database::detail::embraced_column_names<reflection_t>() == "(name,password,karma,cash)");
   }
   SECTION("escape value from colum0")
   {
      using column_t = std::tuple_element_t<0, reflection_t>;
      REQUIRE(database::detail::escape_value_if_necessary<column_t>(obj) == R"('John Doe')");
   }
   SECTION("escape value from colum1")
   {
      using column_t = std::tuple_element_t<1, reflection_t>;
      REQUIRE(database::detail::escape_value_if_necessary<column_t>(obj) == R"('hidden')");
   }
   SECTION("escape value from colum2")
   {
      using column_t = std::tuple_element_t<2, reflection_t>;
      REQUIRE(database::detail::escape_value_if_necessary<column_t>(obj) == "42");
   }
   SECTION("escape value from colum3")
   {
      using column_t = std::tuple_element_t<3, reflection_t>;
      REQUIRE(database::detail::escape_value_if_necessary<column_t>(obj) == "3.14");
   }
   SECTION("embraced_row_values")
   {
      REQUIRE(database::detail::embraced_row_values<reflection_t>(obj) == R"(('John Doe', 'hidden', 42, 3.14))");
   }
}

namespace sec21::database
{
   template <>
   struct column_type<int>
   {
      static constexpr std::string_view value = "INT";
   };
   template <>
   struct column_type<std::string>
   {
      static constexpr std::string_view value = "TEXT";
   };
   template <>
   struct column_type<double>
   {
      static constexpr std::string_view value = "REAL";
   };
} // namespace sec21::database

TEST_CASE("test database queries with 'user' type", "[sec21][database]")
{
   using namespace sec21;

   SECTION("create table")
   {
      const auto result = database::create_table<user>();
      REQUIRE(result ==
              "CREATE TABLE user(name TEXT PRIMARY KEY NOT NULL,password TEXT NOT NULL,karma INT,cash REAL);");
   }
   SECTION("insert values into table")
   {
#if __cpp_designated_initializers >= 201707L
      user obj{.name = "John Doe", .password = "hidden", .karma = 42, .cash = 3.14};
#else
      user obj{"John Doe", "hidden", 42, 3.14};
#endif
      const auto result = database::insert_into(obj);
      REQUIRE(result == R"(INSERT INTO user (name,password,karma,cash) VALUES ('John Doe', 'hidden', 42, 3.14);)");
   }
   SECTION("select all members")
   {
      const auto result = database::select<user>();
      REQUIRE(result == "SELECT name,password,karma,cash FROM user");
   }
}
