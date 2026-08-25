*#include [&lt;sec21/write_csv.h&gt;](https://github.com/MichaelMiller-/sec21/blob/master/include/sec21/write_csv.h)*

Another algorithm that makes use of the reflection library is a [CSV](https://en.wikipedia.org/wiki/Comma-separated_values) writer. Here, the table format is used and written to a stream in a formatted way.

## Example
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
      using name = column<"name", &user::name>;
      using password = column<"password", &user::password>;
      using karma = column<"karma", &user::karma>;
      using cash = column<"cash", &user::cash>;
   };

   using metainfo = std::tuple<
      columns::name,
      columns::password,
      columns::karma,
      columns::cash>;
};

// 
std::vector<user> input{
   { "John Doe", "secret", 42, 0 },
   { "Max Mustermann", "****", 1, 45'678 }
};
sec21::write_csv(std::cout, input);
```

The output is:
```
name, password, karma, cash
John Doe, secret, 42, 0
Max Mustermann, ****, 1, 45678
```




