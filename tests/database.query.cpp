#include <catch.hpp>

#include <sec21/database/table.h>
#include <sec21/database/column.h>
#include <sec21/database/column_type.h>
#include <sec21/database/query.h>

#include <iostream>
#include <tuple>

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
}

struct user
{
   std::string name{};
   std::string password{};
   int karma{};
};

#include <sec21/is_iterator.h>
#include <sec21/database/query.h>

namespace sec21::database
{
   template <>
   struct table<user>
   {
      // using type_t = user;
      static constexpr inline auto name = "user";

      static constexpr auto metainfo() noexcept { return std::tuple{
             column<int, primary_key, not_null>{"id"},
             column<std::string, not_null>{"name"},
             column<std::string, not_null>{"password"},
             column<int>{"karma"},
         }; }

      template <typename... Ts>
      //! \todo argument is declval(table())
      static constexpr auto dispatch(std::tuple<Ts...> const& tuple) noexcept
      {
         return {std::get<2>(tuple), std::get<3>(tuple)};
      }
   };
} // namespace sec21::database

TEST_CASE("test database queries from type: user", "[sec21][database]")
{
   user obj{.name = "John Doe", .password = "geheim", .karma = 42};

   SECTION("select all members")
   {
      const auto result = sec21::database::select<decltype(obj)>();
      REQUIRE(result == "SELECT id,name,password,karma FROM user");
   }
   SECTION("create table")
   {
      const auto result = sec21::database::create_table<decltype(obj)>();
      REQUIRE(result ==  "CREATE TABLE user(id INT PRIMARY KEY NOT NULL,name TEXT NOT NULL,password TEXT NOT NULL,karma INT );");
   }
}

namespace ns
{
   struct foo
   {
      double value;
   };
}

namespace sec21::database 
{
   template <>
   struct table<ns::foo>
   {
      // using type_t = user;
      static constexpr inline auto name = "foo";

      static constexpr auto metainfo() noexcept { return std::tuple{
            column<double, not_null>{"value"},
         }; }

      template <typename... Ts>
      //! \todo argument is declval(table())
      static constexpr auto dispatch(std::tuple<Ts...> const& tuple) noexcept
      {
         return {std::get<0>(tuple)};
      }
   };
} // namespace sec21::database

TEST_CASE("test database queries from type: ns::foo", "[sec21][database]")
{
   ns::foo obj{.value=3.14};

   SECTION("select all members")
   {
      const auto result = sec21::database::select<decltype(obj)>();
      REQUIRE(result == "SELECT value FROM foo");
   }
   SECTION("create table")
   {
      const auto result = sec21::database::create_table<decltype(obj)>();
      REQUIRE(result == "CREATE TABLE foo(value REAL NOT NULL);");
   }
}
// namespace sec21::structural_analysis
//{
//   template <typename Descriptor, typename Precision = double>
//   struct material
//   {
//      using descriptor_t = Descriptor;
//      using precision_t = Precision;
//      using e_modul_t = units::quantity<units::gigapascal, precision_t>;
//      using g_modul_t = units::quantity<units::gigapascal, precision_t>;
//      using second_moment_of_area_t = units::quantity<units::meters_to_the_fourth_power, precision_t>;
//
//      //! \brief unique name
//      descriptor_t name{ descriptor_traits<descriptor_t>::invalid() };
//      //! \brief
//      e_modul_t E{};
//      //! \brief DE: Flächenträgheitsmoment
//      second_moment_of_area_t I{};
//      //! \brief DE: Schubmodul
//      g_modul_t G{};
//   };
//} // namespace sec21::structural_analysis

/*
 *    sqlite3 *db;
    char *err_msg = 0;
    sqlite3_stmt *res;

    int rc = sqlite3_open("test.db", &db);

    if (rc != SQLITE_OK) {

        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);

        return 1;
    }

    char *sql = "SELECT Id, Name FROM Cars WHERE Id = ?";

    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

    if (rc == SQLITE_OK) {

        sqlite3_bind_int(res, 1, 3);
    } else {

        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    }

    int step = sqlite3_step(res);

    if (step == SQLITE_ROW) {

        printf("%s: ", sqlite3_column_text(res, 0));
        printf("%s\n", sqlite3_column_text(res, 1));

    }

    sqlite3_finalize(res);
    sqlite3_close(db);
 *
 */