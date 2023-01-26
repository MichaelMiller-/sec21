
# sec21

[![GCC 10.2](https://github.com/MichaelMiller-/sec21/actions/workflows/gcc.yml/badge.svg)](https://github.com/MichaelMiller-/sec21/actions/workflows/gcc.yml) [![GCC 11.0](https://github.com/MichaelMiller-/sec21/actions/workflows/gcc11.yml/badge.svg)](https://github.com/MichaelMiller-/sec21/actions/workflows/gcc11.yml) [![Clang 12](https://github.com/MichaelMiller-/sec21/actions/workflows/clang12.yml/badge.svg)](https://github.com/MichaelMiller-/sec21/actions/workflows/clang12.yml) [![MSVC 2019](https://github.com/MichaelMiller-/sec21/actions/workflows/msvc.yml/badge.svg)](https://github.com/MichaelMiller-/sec21/actions/workflows/msvc.yml) [![AppVeyor MSVC2019](https://ci.appveyor.com/api/projects/status/4s6bg4yexj0cna45?svg=true)](https://ci.appveyor.com/project/MichaelMiller-/sec21)

[![codecov](https://codecov.io/gh/MichaelMiller-/sec21/branch/master/graph/badge.svg?token=f7vlTsHZWl)](https://codecov.io/gh/MichaelMiller-/sec21) [![CodeQL](https://github.com/MichaelMiller-/sec21/actions/workflows/codeql.yml/badge.svg)](https://github.com/MichaelMiller-/sec21/actions/workflows/codeql.yml)

**sec21** is a collection of useful functions and classes and the boilerplate for a lot of my projects.

- [all_of / any_of](#all_of--any_of)
- [scope_guard](#scope_guard)
- [to_array](#to_array)
- [SQL Database Wrapper](#sql_database_wrapper)
- [arity](#arity)
- [transform](#transform)
- [Eventsystem](#eventsystem)
- [strong_type](#strong_type)
- [expects](#expects)
- [for_each_indexed](#for_each_indexed)
- [units](include/sec21/units/README.md)
- [type_traits](include/sec21/type_traits/README.md)

## Build and install using CMake
Building with CMake on Linux is very easy, just use the following shell commands:
```sh
git clone https://github.com/MichaelMiller-/sec21
cd sec21
mkdir _build && cd _build/
cmake ..
sudo make install
```

## Usage in CMake projects
If you have installed **sec21** system-wide you can use the CMake-package-system.
```cmake
find_package(sec21 CONFIG REQUIRED)
## Required by sec21
find_package(nlohmann_json CONFIG REQUIRED)
## ...
target_link_libraries(${PROJECT_NAME} PRIVATE sec21::sec21)
```


--------------
## all_of / any_of<span id="all_of--any_of"></span>

*#include [<sec21/all_of.h>](include/sec21/all_of.h)* <br/>
*#include [<sec21/any_of.h>](include/sec21/any_of.h)*

Zero overhead syntax helper which also works with lambdas.

### Example
```c++
int x{10};
int y{50};
int z{99};

if (all_of{x, y, z} < 100) {
   // do something
}

auto lambda1 = [](auto e){ return e < 100; };
auto lambda2 = [](auto e){ return e > 5; };
auto lambda3 = [](auto e){ return e > 10; };

auto func = all_of{lambda1, lambda2, lambda3};

if (func(16)) {
   // do something
}
```
--------------
## scope_guard<span id="scope_guard"></span>

*#include [<sec21/scope_guard.h>](include/sec21/scope_guard.h)*

General-purpose scope guard intended to call its exit function when a scope is exited. 

--------------
## to_array<span id="to_array"></span>

*#include [<sec21/to_array.h>](include/sec21/to_array.h)*

Transforms any given std::tuple<Ts...> into a std::array. 

```c++
auto input = std::make_tuple(1, 2, 3, 4.44f, 5.55, "string");
auto result = to_array(input, [](auto v) {
  std::stringstream ss;
  ss << "v: " << v;
  return ss.str();
});

// result is std::array<std::string, 6>{  "v: 1", "v: 2", "v: 3", "v: 4.44", "v: 5.55", "v: string" }; 
```


--------------
## SQL Database Wrapper<span id="sql_database_wrapper"></span>

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
### Dependencies
- [{fmt}](https://github.com/fmtlib/fmt)

--------------
## arity<span id="arity"></span>

*#include [<sec21/arity.h>](include/sec21/arity.h)*

Count the number of arguments of any given callable, including generic lambdas and generic function objects.

### Example
```c++
// plan function
void func(int, int);
static_assert(arity(func) == 2);

// function object
struct binary_functor {
   template <typename T, typename U>
   void operator()(T, U) {}
};
static_assert(arity(binary_functor{}) == 2);

// generic lambda
const auto l3 = [](auto, auto, auto) {};
static_assert(arity(l3) == 3);
```
--------------
## transform<span id="transform"></span>

*#include [<sec21/transform.h>](include/sec21/transform.h)*

Transforms any given std::tuple<Ts...>.

```c++
auto input = std::make_tuple(1, 2, 3, 4.44);
auto result = transform(input, [](auto v) { return v * 2; });
   
// result is std::tuple<int, int, int, double>{ 2, 4, 6, 8.88 }; 
```


--------------
## Eventsystem<span id="eventsystem"></span>

The [input_manager](https://github.com/MichaelMiller-/sec21/blob/master/include/sec21/event/input_manager.h) represents an abstraction layer to system events that are triggered by the hardware (keyboard, mouse, ...). This abstraction allows input events to be simulated using simple input data (JSON files). Making the eventsystem and other systems built on it such as a commmand-queue filled by hardware events very easy to test.

It allows various custom backends. The only limitation is that the backend defines a method poll_event().
This must return a `std::tuple<bool, event_t>` and can take a variable number of parameters. These parameters are passed from the `input_manager` to the backend.

```c++
struct possible_backend
{
   std::tuple<bool, event_t> poll_event();
};

```
The backend should also be able to convert the events provided by the eventsystem into the appropriate backend format. Two sample backends are already implemented: 
- [SDL2](https://www.libsdl.org/)
- [SFML](https://www.sfml-dev.org/)

### Dependencies
- [nlohmann/json](https://github.com/nlohmann/json)

--------------
## strong_type<span id="strong_type"></span>

*#include [<sec21/strong_type.h>](include/sec21/strong_type.h)*

The static typing of the programming language C ++ allows you to design types that exactly meet the specified requirements. Due to the [policy-based design](https://en.wikipedia.org/wiki/Modern_C%2B%2B_Design#Policy-based_design) of 'strong_type' it is very easy to design new types. For example, a counter-like type should be designed which can only be incremented. Trying to decrement an object of this type will result in a compiler error. 
```c++
using counter_t = strong_type<int, struct counter_tag, policy::increment>;

counter_t cnt{ 42 };
++cnt;          // counter_t.value == 43
// --cnt;       // is not possible
```
Due to the specific type, interfaces can be developed much more safely and clearly.
The following example illustrates this well. The interface uses POD data types as function arguments.
When the function is called, the order of the arguments cannot be guaranteed and errors can occur during execution. 
```c++
void set_window_dimension(int width, int height);
// ...
int input_width{ 800 };
int input_height{ 800 };

// interface call
void set_window_dimension(input_width, input_height);
// work's as well
void set_window_dimension(input_height, input_width);
```
A strong_type function argument, however, forces the caller side to explicitly specify the type. Among other things, this also improves the readability of the code. 
```c++
using width_t = strong_type<int, struct width_tag>;
using height_t = strong_type<int, struct height_tag>;

void set_window_dimension(width_t{ 800 }, height_t{ 600 });
```
Furthermore, the behavior and the interface of the strong_type can be extended and tested very easily. The following example shows a type that represents an alphabet. Incrementing above the last letter can throw an error or provide a new value with an index. 

```c++
template <typename T>
// A(0) ... Z(0) ... A1 ... Z1 ... A2 ... Z2 ... 
struct increment_alphabet
{
   T& operator ++ () noexcept
   {
      using type_t = typename T::underlying_t;
      static_assert(std::is_same_v<type_t, std::string>);

      // get reference to the underlaying type
      auto& result = static_cast<type_t&>(static_cast<T&>(*this));

      // modify value
      if (result[0] == 'Z') {
         const auto tmp = type_t{ next(begin(result)), end(result) };
         unsigned long long number{0};
         std::from_chars(tmp.data(), tmp.data() + tmp.size(), number);
         result = fmt::format("A{}", ++number);
      }
      else
         ++result[0];

      return static_cast<T&>(*this);
   }
};

using alhpabet_t = strong_type<
   std::string, 
   struct alhpabet_tag, 
   increment_alphabet, 
   policy::compare>;

alhpabet_t obj{ "Z" };
++obj;
REQUIRE(obj == "A1");
```

--------------
## expects<span id="expects"></span>

*#include [<sec21/expects.h>](include/sec21/expects.h)*

A simple one-liner that provides general error handling. If the condition evaluates to false, a centrally defined action is executed with the passed text.

```cpp
// if the condition is false a std::runtime_error is thrown
expects([=]{ return value > 23; }, "value has to be greater than 23");
```


--------------
## for_each_indexed<span id="for_each_indexed"></span>

*#include [<sec21/for_each_indexed.h>](include/sec21/for_each_indexed.h)*

Executes the passed function per element with the associated index. There is another overload where the start index can be specified.

```c++
auto input = std::vector{ 4, 3, 2, 1 };
for_each_indexed(
   begin(input), 
   end(input), 
   [](auto index, auto value) { std::cout << "i: " << index << " v: " << value << std::endl; });

// output:
// i: 0 v: 4  
// i: 1 v: 3  
// i: 2 v: 2  
// i: 3 v: 1  
```

