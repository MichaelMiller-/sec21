*#include [&lt;sec21/meta/contains.h&gt;](https://github.com/MichaelMiller-/sec21/blob/master/include/sec21/meta/contains.h)*

Check if a type-list contains a given type

### Example
```c++
using list_t = type_list<int, float, char>;
static_assert(contains<int, list_t>::value == true);

using tuple_list_t = std::tuple<int, float, char>;
static_assert(contains<int, tuple_list_t>::value == true);

using variant_list_t = std::variant<int, float, char>;
static_assert(contains<int, variant_list_t>::value == true);
```
