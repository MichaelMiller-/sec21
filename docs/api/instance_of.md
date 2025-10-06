*#include [&lt;sec21/type_traits/instance_of.h&gt](https://github.com/MichaelMiller-/sec21/blob/master/include/sec21/type_traits/instance_of.h)*

Checks whether a type corresponds to an instantiation of a class.

### Example
```c++
static_assert(is_instance_of_v<std::basic_string, std::string> == true);
static_assert(is_instance_of_v<std::tuple, std::tuple<int, double>> == true);
static_assert(is_instance_of_v<std::tuple, std::vector<int>> == false);
static_assert(is_instance_of_v<std::vector, std::tuple<int, double>> == false);
```
