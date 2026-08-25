*#include [&lt;sec21/meta/push_front.h&gt;](https://github.com/MichaelMiller-/sec21/blob/master/include/sec21/meta/push_front.h)*

Push a given type to the front of any list

### Example
```c++
using T = type_list<int, float>;
using R = push_front_t<char, T>;
static_assert(std::is_same_v<R, type_list<char, int, float>>);
```
