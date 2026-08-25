*#include [&lt;sec21/meta/sort.h&gt;](https://github.com/MichaelMiller-/sec21/blob/master/include/sec21/meta/sort.h)*

Sort any given type-list with a binary predicate.

### Example
```c++
template <typename Lhs, typename Rhs>
struct predicate : std::bool_constant<sizeof(Lhs) < sizeof(Rhs)>
{
};

using T = type_list<int, float, char, double, short>;
using R = sort<T, predicate>::type;

static_assert(std::is_same_v<R, type_list<char, short, int, float, double>>);
```
