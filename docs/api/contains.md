*#include [&lt;sec21/type_traits/contains.h&gt;](https://github.com/MichaelMiller-/sec21/blob/master/include/sec21/type_traits/contains.h)*

Template which checks whether an explicit type is in a type list. 

### Example
```c++
using type_list_t = std::tuple<int, float, double>;

static_assert(contains<int, type_list_t>::value == true);
static_assert(contains<float, type_list_t>::value == true);
static_assert(contains<double, type_list_t>::value == true);
static_assert(contains<char, type_list_t>::value == false);
```

