*#include [&lt;sec21/meta/merge.h&gt;](https://github.com/MichaelMiller-/sec21/blob/master/include/sec21/meta/merge.h)*

Merge several type-list into one list

### Example
```c++
using T1 = std::tuple<int>;
using T2 = std::tuple<float>;
using T3 = std::tuple<char>;
using R = merge<T1, T2, T3>::type;

static_assert(std::is_same_v<R, std::tuple<int, float, char>>);
```
