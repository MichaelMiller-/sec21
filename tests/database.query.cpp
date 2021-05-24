#include <catch.hpp>

#include <sec21/database/table.h>
#include <sec21/database/column.h>
#include <sec21/database/column_type.h>
#include <sec21/database/query.h>

#include <iostream>
#include <tuple>

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

// begin-snippet: db.user.example
struct user
{
   std::string name{};
   std::string password{};
   int karma{};
   double cash{}; 
};
// end-snippet

// #if __cpp_nontype_template_args >= 201911L

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

TEST_CASE("test detail implementation", "[sec21][database]")
{
   using namespace sec21::database;

#ifdef __cpp_designated_initializers
   user obj{.name = "John Doe", .password = "hidden", .karma = 42, .cash = 3.14};
#else
   user obj{"John Doe", "hidden", 42, 3.14};
#endif

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
#ifdef __cpp_designated_initializers
      user obj{.name = "John Doe", .password = "hidden", .karma = 42, .cash = 3.14};
#else
      user obj{"John Doe", "hidden", 42, 3.14};
#endif      
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
// #endif

struct example
{
   std::string name{};
   int karma{};
   std::array<double,2> position{};
   double cash{}; 
};

namespace sec21::database
{
   template <>
   struct table<example>
   {
      static constexpr inline auto name = "example";

      struct columns
      {
         struct name {
            using value_t = std::string;
            using constraints_t = std::tuple<serial, primary_key>;
            static constexpr inline auto column_name = "name";
            static value_t get(example const& obj) { return obj.name; } 
         };
         struct pos_x {
            using value_t = double;
            using constraints_t = std::tuple<>;
            static constexpr inline auto column_name = "pos_x";
            static value_t get(example const& obj) { return std::get<0>(obj.position); } 
         };        
         struct pos_y {
            using value_t = double;
            using constraints_t = std::tuple<>;
            static constexpr inline auto column_name = "pos_y";
            static value_t get(example const& obj) { return std::get<1>(obj.position); } 
         };        
         struct cash {
            using value_t = double;
            using constraints_t = std::tuple<>;
            static constexpr inline auto column_name = "cash";
            static value_t get(example const& obj) { return obj.cash; }
         }; 
         struct karma {
            using value_t = int;
            using constraints_t = std::tuple<>;
            static constexpr inline auto column_name = "karma";
            static value_t get(example const& obj) { return obj.karma; }           
         };
      };
      using metainfo = std::tuple<
         columns::name,
         columns::karma,
         columns::pos_x,
         columns::pos_y,
         columns::cash>;
   };
} // namespace sec21::database

#include <fmt/format.h>
#include <fmt/ranges.h>

namespace v2
{
#if 1 
   template<typename...Ts, typename Function, size_t... Is>
   constexpr auto transform_impl(std::tuple<Ts...> const& inputs, Function function, std::index_sequence<Is...>)
   {
      return std::tuple<std::result_of_t<Function(Ts)>...>{function(std::get<Is>(inputs))...};
   }

   template<typename... Ts, typename Function>
   constexpr auto transform(std::tuple<Ts...> const& inputs, Function function)
   {
      return transform_impl(inputs, function, std::make_index_sequence<sizeof...(Ts)>{});
   }
#else
   template <typename Tuple, typename Callable, size_t... Is>
   [[nodiscard]] auto transform(Tuple const& inputs, Callable&& func, std::index_sequence<Is...> = {})
   {
      if (std::tuple_size<Tuple>::value == sizeof...(Is)) {
         // return {std::get<Is>(t)...};
         return std::tuple<std::result_of_t<Callable(std::tuple_element<Is>::type)>>{function(std::get<Is>(inputs))...};
      }
      return transform(inputs, std::forward<Callable>(func), std::make_index_sequence<std::tuple_size<Tuple>::value>{});
   }
#endif
   template <typename Table>
   auto column_names()
   {
      constexpr auto names = transform(Table{}, [](auto v){ return decltype(v)::column_name; });
      return fmt::format("{}", fmt::join(names, ","));
   }

   template <typename Row>
   auto insert_into(Row const& row)
   {
      using reflection_t = typename sec21::database::table<Row>::metainfo;

      const auto values = transform(reflection_t{}, [obj = row](auto v)
      { 
         if constexpr (std::is_convertible_v<typename decltype(v)::value_t, std::string>) {
               return fmt::format("'{}'", decltype(v)::get(obj));
         }
         return decltype(v)::get(obj); 
      });
      return fmt::format("INSERT INTO {} ({}) VALUES ({});",
         sec21::database::table<Row>::name,
         column_names<reflection_t>(),
         fmt::join(values, ","));
   }

   template <typename Table>
   auto create_table()
   {
      using reflection_t = typename sec21::database::table<Table>::metainfo;

      const auto types = transform(reflection_t{}, [](auto v)
      { 
         constexpr auto constrains = transform(typename decltype(v)::constraints_t{}, [](auto y){ 
            return decltype(y)::value; 
         });

         return fmt::format("{} {} {}", 
               decltype(v)::column_name, 
               sec21::database::column_type<typename decltype(v)::value_t>::name,
               fmt::join(constrains, " ")
         );
      });
      return fmt::format("CREATE TABLE {}({});", sec21::database::table<Table>::name, fmt::join(types, ","));
   }
}

TEST_CASE("test example", "[sec21][database]")
{
   using namespace sec21::database;

   example obj{ "foo", 42, { 3.14, 1.23 }, 22500 };

   using reflection_t = table<decltype(obj)>::metainfo;

   SECTION("column names")
   {
      const auto result = v2::column_names<reflection_t>();
      const std::string expected = R"(name,karma,pos_x,pos_y,cash)";
      REQUIRE(result == expected);
   }
   SECTION("create table")
   {
      const auto result = v2::create_table<example>();
      REQUIRE(result == "CREATE TABLE example(name TEXT SERIAL PRIMARY KEY,karma INT ,pos_x REAL ,pos_y REAL ,cash REAL );");
   }   
   SECTION("insert values into table")
   {
      const auto result = v2::insert_into(obj);
      REQUIRE(result == R"(INSERT INTO example (name,karma,pos_x,pos_y,cash) VALUES ('foo',42,3.14,1.23,22500.0);)");
   }   
}