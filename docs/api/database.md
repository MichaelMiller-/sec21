## SQL Database Wrapper <span id="database"></span>

Is a simple database wrapper that allows, via a reflection mechanism, to interpret data structures as SQL tables.  
```cpp
struct user
{
   std::string name{};
   std::string password{};
   int karma{};
   double cash{}; 
};
```
The original data structure is not changed. Instead, the table reflection is specified by another class. Thanks to the template specialization of the ```database::table``` class, all columns of the table are made known.
```cpp
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
```

This procedure enables type-safe SQL statements to be set up and used. The following code shows, for example, the insertion of C++ objects into the database table specified above.
```cpp
user obj{.name = "John Doe", .password = "hidden", .karma = 42, .cash = 3.14};
const auto result = insert_into(obj);

REQUIRE(result == R"(INSERT INTO user (name,password,karma,cash) VALUES ('John Doe','hidden',42,3.14);)");
```

### TODO
### Dependencies
- [{fmt}](https://github.com/fmtlib/fmt)
