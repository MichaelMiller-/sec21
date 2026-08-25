*#include [&lt;sec21/type_traits/is_tuple.h&gt;](https://github.com/MichaelMiller-/sec21/blob/master/include/sec21/type_traits/is_tuple.h)*

Checks whether the type is a std::tuple<Ts...>.

### Example
```c++
static_assert(is_tuple_v<std::tuple<int, float, int>> == true);
static_assert(is_tuple_v<std::tuple<>> == true);
static_assert(is_tuple_v<int> == false);
static_assert(is_tuple_v<std::string> == false);
static_assert(is_tuple_v<double> == false);
```
