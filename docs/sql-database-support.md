With the help of the [reflection](https://github.com/MichaelMiller-/sec21/wiki/Reflection.md) library, it is very easy to map data structures as SQL tables.

```cpp
struct user
{
   std::string name{};
   std::string password{};
   int karma{};
   double cash{}; 
};

template <>
struct sec21::reflection::table<user>
{
   static constexpr auto name = "user";

   struct columns
   {
      using name = column<"name", &user::name, database::primary_key, database::not_null>;
      using password = column<"password", &user::password, database::not_null>;
      using karma = column<"karma", &user::karma>;
      using cash = column<"cash", &user::cash>;
   };

   using metainfo = std::tuple<
      columns::name,
      columns::password,
      columns::karma,
      columns::cash>;
};
```

As you can see the original data structure is not changed. Instead, the table is specified by the template specialization of the ```reflection::table``` class. In addition, the first two columns are given freely selectable keywords as constraints. This is followed by a data type mapping for the corresponding database dialect. In this case, [PostgreSQL](https://www.postgresql.org/) data types are mapped.


```cpp
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
```

In the database library there are various functions to generate SQL statements.

```cpp
{
  auto sql_statement = sec21::database::create_table<user>();

  // sql_statement = "CREATE TABLE user(name TEXT PRIMARY KEY NOT NULL,password TEXT NOT NULL,karma INT,cash REAL);"
}
{
  user obj{.name = "John Doe", .password = "hidden", .karma = 42, .cash = 3.14};
  auto sql_statement = sec21::database::insert_into(obj);

  // sql_statement == "INSERT INTO user (name,password,karma,cash) VALUES ('John Doe','hidden',42,3.14);"
}
```
