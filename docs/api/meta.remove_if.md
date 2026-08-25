*#include [&lt;sec21/meta/remove_if.h&gt;](https://github.com/MichaelMiller-/sec21/blob/master/include/sec21/meta/remove_if.h)*

Remove a type that satisfy a predicate from a type-list

### Example
```c++
template <typename T>
struct predicate : std::bool_constant<sizeof(T) == 4>
{
};

using T = type_list<int, float, char>;
using R = remove_if<T, predicate>::type;

static_assert(std::is_same_v<R, type_list<char>>);
```
