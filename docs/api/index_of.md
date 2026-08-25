*#include [&lt;sec21/type_traits/index_of.h&gt;](https://github.com/MichaelMiller-/sec21/blob/master/include/sec21/type_traits/index_of.h)*

Calculates the index of a type in a type list.

### Example
```c++
static_assert(index_of<int, int, float, char>::value == 0);
static_assert(index_of<float, int, float, char>::value == 1);
static_assert(index_of<char, int, float, char>::value == 2);
static_assert(index_of<double, int, float, char>::value == -1);   // -1 == not found
```
