*#include [&lt;sec21/type_traits/is_std_string.h&gt;](https://github.com/MichaelMiller-/sec21/blob/master/include/sec21/type_traits/is_std_string.h)*

Checks whether the type is a std::string.

### Example
```c++
static_assert(is_std_string<std::string>::value == true);
static_assert(is_std_string<int>::value == false);
static_assert(is_std_string<std::wstring>::value == false);
static_assert(is_std_string<float>::value == false);
```
