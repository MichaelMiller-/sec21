*#include [&lt;sec21/concat.h&gt;](https://github.com/MichaelMiller-/sec21/blob/master/include/sec21/concat.h)*

Joins string-convertible objects into one string. There are overloads for std::tuple and variadic template parameters. It is also possible to specify the delimiter between the objects.

```cpp
auto input = std::make_tuple("foo", "bar", "baz");
auto result = concat(input, "+++");  // == "foo+++bar+++baz");
```
