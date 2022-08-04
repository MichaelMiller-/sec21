
# sec21

[![GCC 10.2](https://github.com/MichaelMiller-/sec21/actions/workflows/gcc.yml/badge.svg)](https://github.com/MichaelMiller-/sec21/actions/workflows/gcc.yml) [![GCC 11.0](https://github.com/MichaelMiller-/sec21/actions/workflows/gcc11.yml/badge.svg)](https://github.com/MichaelMiller-/sec21/actions/workflows/gcc11.yml) [![Clang 12](https://github.com/MichaelMiller-/sec21/actions/workflows/clang12.yml/badge.svg)](https://github.com/MichaelMiller-/sec21/actions/workflows/clang12.yml) [![MSVC 2019](https://github.com/MichaelMiller-/sec21/actions/workflows/msvc.yml/badge.svg)](https://github.com/MichaelMiller-/sec21/actions/workflows/msvc.yml) [![AppVeyor MSVC2019](https://ci.appveyor.com/api/projects/status/4s6bg4yexj0cna45?svg=true)](https://ci.appveyor.com/project/MichaelMiller-/sec21)

**sec21** is a collection of useful functions and classes.

- [all_of / any_of ](#allanyof)
- [scope_guard ](#scopeguard)
- [SQL Database Wrapper ](#database)
- [arity ](#arity)
- [Eventsystem ](#events)
- [strong_type ](#strongtype)
- [expects ](#expects)
- [units](include/sec21/units/README.md)
- [type_traits](include/sec21/type_traits/README.md)

### Examples
- [viewer](viewer/README.md)

## Build and install using CMake
Building with cmake on Linux is easy, just use the following:
```sh
git clone https://github.com/MichaelMiller-/sec21
cd sec21 
mkdir _build && cd _build/
cmake ..
sudo make install
```
In case any dependencies are missing, cmake will tell you.


--------------
## all_of / any_of <span id="allanyof"></span>

- [all_of.h](include/sec21/all_of.h)
- [any_of.h](include/sec21/any_of.h)

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
## scope_guard <span id="scopeguard"></span>

[Header](include/sec21/scope_guard.h)

General-purpose scope guard intended to call its exit function when a scope is exited. 

--------------
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
### Dependencies
- [{fmt}](https://github.com/fmtlib/fmt)

--------------
## arity <span id="arity"></span>

[Header](include/sec21/arity.h)

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
## Eventsystem <span id="events"></span>

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
## strong_type <span id="strongtype"></span>

[Header](include/sec21/strong_type.h)

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
## expects <span id="expects"></span>

*#include [<sec21/expects.h>](include/sec21/expects.h)*

A simple one-liner that provides general error handling. If the condition evaluates to false, a centrally defined action is executed with the passed text.

```cpp
// if the condition is false a std::runtime_error is thrown
expects([=]{ return value > 23; }, "value has to be greater than 23");
```


