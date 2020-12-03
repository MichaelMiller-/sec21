#include <catch.hpp>

#include <sec21/database/table.h>
#include <sec21/database/column.h>
#include <sec21/database/column_type.h>
#include <sec21/database/query.h>

#include <iostream>
#include <tuple>

// begin-snippet: db.user.example
struct user
{
   std::string name{};
   std::string password{};
   int karma{};
   double cash{}; 
};
// end-snippet

namespace sec21::database
{
   template <>
   struct column_type<int>
   {
      static constexpr inline auto name = "INT";
   };
   template <>
   struct column_type<std::string>
   {
      static constexpr inline auto name = "TEXT";
   };
   template <>
   struct column_type<double>
   {
      static constexpr inline auto name = "REAL";
   };

   // begin-snippet: db.user.reflection
   template <>
   struct table<user>
   {
      static constexpr inline auto name = "user";

      struct columns
      {
         using name = column<"name", user, std::string, &user::name, primary_key, not_null>;
         using password = column<"password", user, std::string, &user::password, not_null>;
         using karma = column<"karma", user, int, &user::karma>;
         using cash = column<"cash", user, double, &user::cash>;
      };

      using metainfo = std::tuple<
         columns::name,
         columns::password,
         columns::karma,
         columns::cash>;
   };
   // end-snippet
} // namespace sec21::database

TEST_CASE("concat column constraints", "[sec21][database]")
{
   using namespace sec21::database;

   SECTION("empty list")
   {
      using input_t = std::tuple<>;
      constexpr auto indices = std::make_index_sequence<std::tuple_size_v<input_t>>{};

      std::stringstream ss;
      detail::concat_constraints<input_t>(ss, indices);

      REQUIRE(std::empty(ss.str()));
   }
   SECTION("list with one element")
   {
      using input_t = std::tuple<not_null>;
      constexpr auto indices = std::make_index_sequence<std::tuple_size_v<input_t>>{};

      std::stringstream ss;
      detail::concat_constraints<input_t>(ss, indices);
      //! \todo leading whitespace is kind of awkward
      REQUIRE(ss.str() == " NOT NULL");
   }
   SECTION("list with two elements")
   {
      using input_t = std::tuple<primary_key, not_null>;
      constexpr auto indices = std::make_index_sequence<std::tuple_size_v<input_t>>{};

      std::stringstream ss;
      detail::concat_constraints<input_t>(ss, indices);
      //! \todo leading whitespace is kind of awkward
      REQUIRE(ss.str() == " PRIMARY KEY NOT NULL");
   }
}

TEST_CASE("test detail implementation", "[sec21][database]")
{
   using namespace sec21::database;

   user obj{.name = "John Doe", .password = "hidden", .karma = 42, .cash = 3.14};

   using reflection_t = table<decltype(obj)>::metainfo;
   constexpr auto indices = std::make_index_sequence<std::tuple_size_v<reflection_t>>{};

   SECTION("column_names")
   {
      std::stringstream ss;
      detail::column_names<reflection_t>(ss, indices);

      REQUIRE(ss.str() == R"(name,password,karma,cash)");
   }
   SECTION("embraced_column_names")
   {
      std::stringstream ss;
      detail::embraced_column_names<reflection_t>(ss, indices);

      REQUIRE(ss.str() == R"((name,password,karma,cash))");
   }
   SECTION("escape value from colum0")
   {
      using colum_t = std::tuple_element_t<0, reflection_t>;
      std::stringstream ss;
      detail::escape_value_if_necessary<colum_t>(ss, obj);

      REQUIRE(ss.str() == R"('John Doe')");
   }
   SECTION("escape value from colum1")
   {
      using colum_t = std::tuple_element_t<1, reflection_t>;
      std::stringstream ss;
      detail::escape_value_if_necessary<colum_t>(ss, obj);

      REQUIRE(ss.str() == R"('hidden')");
   }
   SECTION("escape value from colum2")
   {
      using colum_t = std::tuple_element_t<2, reflection_t>;
      std::stringstream ss;
      detail::escape_value_if_necessary<colum_t>(ss, obj);

      REQUIRE(ss.str() == R"(42)");
   }
   SECTION("escape value from colum3")
   {
      using colum_t = std::tuple_element_t<3, reflection_t>;
      std::stringstream ss;
      detail::escape_value_if_necessary<colum_t>(ss, obj);

      REQUIRE(ss.str() == R"(3.14)");
   }
   SECTION("embraced_row_values")
   {
      std::stringstream ss;
      detail::embraced_row_values<reflection_t>(ss, obj, indices);

      REQUIRE(ss.str() == R"(('John Doe','hidden',42,3.14))");
   }   
}

TEST_CASE("test database queries from type: user", "[sec21][database]")
{
   using namespace sec21::database;

   SECTION("create table")
   {
      const auto result = create_table<user>();
      REQUIRE(result == "CREATE TABLE user(name TEXT PRIMARY KEY NOT NULL,password TEXT NOT NULL,karma INT,cash REAL);");
   }
   SECTION("insert values into table")
   {
      // begin-snippet: db.insert
      user obj{.name = "John Doe", .password = "hidden", .karma = 42, .cash = 3.14};
      const auto result = insert_into(obj);

      REQUIRE(result == R"(INSERT INTO user (name,password,karma,cash) VALUES ('John Doe','hidden',42,3.14);)");
      // end-snippet
   }
   SECTION("select all members")
   {
      const auto result = select<user>();

      REQUIRE(result == "SELECT name,password,karma,cash FROM user");
   }
}
