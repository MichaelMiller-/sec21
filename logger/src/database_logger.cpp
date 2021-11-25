#include "database_logger.h"

//! \todo refactor: instead of write a file push the messages into a database
// create type log_entry_kind_enum as enum ('Info', 'Error', 'Warning', 'Metric', 'Debug');

using namespace sec21::log;

void database_logger::log(entry const& )
{

}


/*
 *

To expand on MPelletier’s answer, you can create the tables like so:

CREATE TABLE Price (
  PriceId INTEGER       PRIMARY KEY AUTOINCREMENT NOT NULL,
  Name    VARCHAR(100)  NOT NULL,
  Type    CHAR(1)       NOT NULL DEFAULT ('M') REFERENCES PriceType(Type)
);

CREATE TABLE PriceType (
  Type    CHAR(1)       PRIMARY KEY NOT NULL,
  Seq     INTEGER
);
INSERT INTO PriceType(Type, Seq) VALUES ('M',1);
INSERT INTO PriceType(Type, Seq) VALUES ('R',2);
INSERT INTO PriceType(Type, Seq) VALUES ('H',3);

Now the enumeration values are available directly in the Price table as they would be using an ENUM: you don’t need to join to the PriceType table to get the Type values, you only need to use it if you want to determine the sequence of the ENUMs.

Foreign key constraints were introduced in SQLite version 3.6.19.

 */

/*
 * CREATE TABLE prices (
 id         INTEGER                                PRIMARY KEY,
 pName      TEXT CHECK( LENGTH(pName) <= 100 )     NOT NULL DEFAULT '',
 pType      TEXT CHECK( pType IN ('M','R','H') )   NOT NULL DEFAULT 'M',
 pField     TEXT CHECK( LENGTH(pField) <= 50 )     NULL DEFAULT NULL,
 pFieldExt  TEXT CHECK( LENGTH(pFieldExt) <= 50 )  NULL DEFAULT NULL,
 cmp_id     INTEGER                                NOT NULL DEFAULT '0'
)
 */

/*
enum class A
{
    A,B,C,D
};
struct example
{
   std::string message{};
   int karma{};
    A a{ A::A };
    int process_id{-1};
    long timestamp{0};
};

namespace sec21::database
{
    template <typename T>
    struct table;

   template <typename T>
   struct column_type;
}
namespace sec21::database
{
   struct primary_key {
      static inline constexpr auto value = "PRIMARY KEY";
   };
    struct auto_increment {
      static inline constexpr auto value = "AUTOINCREMENT";
   };
   struct not_null {
      static inline constexpr auto value = "NOT NULL";
   };

   template <>
   struct table<example>
   {
      static constexpr inline auto name = "example";

      struct columns
      {
         struct id
         {
            using value_t = std::int64_t;
            using constraints_t = std::tuple<primary_key, not_null, auto_increment>;
            static constexpr inline auto column_name = "id";
            // static value_t get(example const& obj) { return obj.name; }
         };
         struct kind
         {
            using value_t = int;
            using constraints_t = std::tuple<not_null>;
            static constexpr inline auto column_name = "kind";
            static value_t get(example const& obj) { return obj.process_id; }
         };
         struct process_id
         {
            using value_t = int;
            using constraints_t = std::tuple<not_null>;
            static constexpr inline auto column_name = "process_id";
            static value_t get(example const& obj) { return obj.process_id; }
         };
         struct timestamp
         {
            using value_t = int;
            using constraints_t = std::tuple<not_null>;
            static constexpr inline auto column_name = "timestamp";
            static value_t get(example const& obj) { return obj.timestamp; }
         };
         struct message
         {
            using value_t = std::string;
            using constraints_t = std::tuple<not_null>;
            static constexpr inline auto column_name = "message";
            static value_t get(example const& obj) { return obj.message; }
         };
    };
      using metainfo = std::tuple<columns::id, columns::kind, columns::process_id, columns::timestamp, columns::message>;
   };
} // namespace sec21::database

#include <fmt/format.h>
#include <fmt/ranges.h>

namespace sec21::database
{
   template <typename... Ts, typename Callable>
   constexpr auto transform(std::tuple<Ts...> const& inputs, Callable function)
   {
      return [&]<size_t... Is>(std::index_sequence<Is...>) noexcept(noexcept(std::is_nothrow_invocable_v<Callable, Ts...>))
      {
         return std::tuple<std::result_of_t<Callable(Ts)>...>{function(std::get<Is>(inputs))...};
      }
      (std::make_index_sequence<sizeof...(Ts)>{});
   }

   template <typename Table>
   auto column_names()
   {
      constexpr auto names = transform(Table{}, [](auto v) { return decltype(v)::column_name; });
      return fmt::format("{}", fmt::join(names, ","));
   }

   template <>
   struct column_type<int> // int32_t
   {
      static constexpr inline auto name = "INT";
#if 0
      static auto invoke(sqlite3_stmt* stmt, int column)
      {
         if (!stmt)
            throw std::invalid_argument("invalid statement pointer");
         // sqlite3_column_int -> 32-bit INTEGER result
         return sqlite3_column_int(stmt, column);
      }
#endif
   };
    template <>
   struct column_type<std::int64_t>
   {
      static constexpr inline auto name = "INTEGER";
#if 0
      static auto invoke(sqlite3_stmt* stmt, int column)
      {
         if (!stmt)
            throw std::invalid_argument("invalid statement pointer");
         // sqlite3_column_int64 -> 64-bit INTEGER result
         return sqlite3_column_int64(stmt, column);
      }
#endif
   };
   template <>
   struct column_type<std::string>
   {
      static constexpr inline auto name = "TEXT";
#if 0
      static auto invoke(sqlite3_stmt* stmt, int column)
      {
         if (!stmt)
            throw std::invalid_argument("invalid statement pointer");

         // sqlite3_column_text -> UTF-8 TEXT result
         const auto value = sqlite3_column_text(stmt, column);
         return std::string{ reinterpret_cast<const char*>(value) };
      }
#endif
   };
   template <>
   struct column_type<double>
   {
      static constexpr inline auto name = "REAL";
#if 0
      static auto invoke(sqlite3_stmt* stmt, int column)
      {
         if (!stmt)
            throw std::invalid_argument("invalid statement pointer");
         // sqlite3_column_double -> REAL result
         return sqlite3_column_double(stmt, column);
      }
      #endif
   };

namespace sqlite3
{
   template <typename Table>
   auto create_table()
   {
      using reflection_t = typename table<Table>::metainfo;

      const auto types = transform(reflection_t{}, [](auto v) {
        constexpr auto constrains =
        transform(typename decltype(v)::constraints_t{}, [](auto y) { return decltype(y)::value; });

         return fmt::format("{} {} {}", decltype(v)::column_name, column_type<typename decltype(v)::value_t>::name, fmt::join(constrains, " "));
      });
      return fmt::format("CREATE TABLE {}({});", table<Table>::name, fmt::join(types, ","));
   }
}
} // namespace v2
 */
