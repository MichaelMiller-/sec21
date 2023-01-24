## sec21::units <span id="units"></span>

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