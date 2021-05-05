# sec21

Linux [![Build Status](https://travis-ci.org/MichaelMiller-/sec21.svg?branch=master)](https://travis-ci.org/MichaelMiller-/sec21)  Windows [![Build status](https://ci.appveyor.com/api/projects/status/4s6bg4yexj0cna45?svg=true)](https://ci.appveyor.com/project/MichaelMiller-/sec21)

**sec21** is a collection of useful functions and classes.
- [Units](#Units)
- [Eventsystem](#Eventsystem)
- [SQL Database Wrapper](include/sec21/database/README.md)
- [strong_type](#strong_type)


Examples 
- [Viewer](viewer/README.md)

---------------------------------------
## Units
Is a modern, lightweight library for [dimensional analysis](https://en.wikipedia.org/wiki/Dimensional_analysis). 
Inspired by [Boost](https://www.boost.org)-Units and Boost-MPL. 
In order to be able to use their full range of functions, only one include is required. 
```
#include <sec21/units.h>
```

The library is also very easy to expand. If a dimension is missing, it can be created very easily and made known to the system.
The following is an example of [pressure](https://en.wikipedia.org/wiki/Pressure), the dimension of which is equal to **M L^-1 T^-2**. 
```c++
using pressure = dimension<
   exponent<base_dimension_mass, 1>, 
   exponent<base_dimension_length, -1>,
   exponent<base_dimension_time, -2>>;
```
The basic dimensions represent freely chosen compiletime constants. 
The procedure is similarly simple for new units. These just have to be defined and derived from [derived_unit](https://github.com/MichaelMiller-/sec21/blob/master/include/sec21/units/unit.h).

```c++
struct pascal : derived_unit<pascal, pressure, base_unit> {};
struct kilopascal : derived_unit<kilopascal, pressure, std::kilo> {};
// ...
```

### Dependencies
- [Boost.Mp11](https://www.boost.org/doc/libs/1_74_0/libs/mp11/doc/html/mp11.html)
- [nlohmann/json](https://github.com/nlohmann/json)


---------------------------------------
## Eventsystem

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


---------------------------------------
## strong_type
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
