# sec21

Linux [![Build Status](https://travis-ci.org/MichaelMiller-/sec21.svg?branch=master)](https://travis-ci.org/MichaelMiller-/sec21)  Windows [![Build status](https://ci.appveyor.com/api/projects/status/4s6bg4yexj0cna45?svg=true)](https://ci.appveyor.com/project/MichaelMiller-/sec21)

**sec21** is a collection of useful functions and classes.
- [Units](#Units)
- [Eventsystem](#Eventsystem)
- [SQL Database Wrapper](include/sec21/database/README.md)

Examples 
- [Viewer](#Viewer)

---------------------------------------
## Units

Modern header-only library for [dimensional analysis](https://en.wikipedia.org/wiki/Dimensional_analysis) is inspired by [Boost](https://www.boost.org)-Units and Boost-MPL. Only a single header has to be included in order to use the library.
```
#include <sec21/units.h>
```

If any dimension is missing it is easy to add a new one to the system. Here is an example for [pressure](https://en.wikipedia.org/wiki/Pressure). The dimension is equivalent to **M L^-1 T^-2**.

```c++
using pressure = dimension<
   exponent<base_dimension_mass, 1>, 
   exponent<base_dimension_length, -1>,
   exponent<base_dimension_time, -2>>;
```
To add new units simple define them and inherit form [derived_unit](https://github.com/MichaelMiller-/sec21/blob/master/include/sec21/units/unit.h).

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
## Viewer
The Viewer has it's own [description](viewer/README.md)

