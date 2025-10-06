# sec21::reflection

The idea behind this library is not to change existing data structures, but to achieve reflection functionality via a simple template specialization. 

> **Note:** 
> Based on this development, another library has been created that makes use of the [Reflection TS](https://en.cppreference.com/w/cpp/experimental/reflect). Feel free to check it out. [Link](https://github.com/MichaelMiller-/out)

## Example
```cpp
// example class / existing code
struct user
{
   std::string name{};
   std::string password{};
   int karma{};
   double cash{}; 
};

struct rectangle
{
    double width{};
    double height{};
};
```

The existing classes can be made known to the system via template specialization.

```cpp
// specialize 'sec21::reflection::table' template for those classes
//
template <>
struct sec21::reflection::table<user>
{
   static constexpr auto name = "user";

   struct columns
   {
      using name = column<"name", &user::name, primary_key, not_null>;
      using password = column<"password", &user::password, not_null>;
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
template <>
struct sec21::reflection::table<rectangle>
{
   static constexpr auto name = "rectangle";

   struct columns
   {
      using width = column<"width", &rectangle::width>;
      using height = column<"height", &rectangle::height>;
   };

   using metainfo = std::tuple<columns::width, columns::height>;
};
```

Once the classes have been made known to the [reflection](https://en.wikipedia.org/wiki/Reflective_programming) system, it is very easy to develop generic algorithms that take advantage of the system.


```cpp
// generic algorithm that works with any type that provides a reflection specialization
template <typename T>
void print_first_column(std::vector<T> const& values)
{
    using namespace sec21;
    using reflection_t = typename reflection::table<T>::metainfo;
    for (auto const& value : values)
    {
        std::cout << reflection::get_column<0>(value) << std::endl;
    }    
}

int main()
{
    {
        std::vector<user> input{
            { "John Doe", "secret", 42, 0 },
            { "Max Mustermann", "****", 1, 45'678 }
        };
        print_first_column(input);
    }
    {
        std::vector<rectangle> input{
            { 3.14, 4.15 },
            { 1.41, 2.77 }
        };
        print_first_column(input);
    }
   return 0;
```

The output is:
```
John Doe
Max Mustermann
3.14
1.41
```
