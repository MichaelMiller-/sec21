# sec21::units

Modern header-only library for [dimensional analysis](https://en.wikipedia.org/wiki/Dimensional_analysis) is inspired by [Boost](https://www.boost.org)-Units and Boost-MPL. In Order to use the library only a single header has to be included.
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