*#include [&lt;sec21/fixed_string.h&gt;](https://github.com/MichaelMiller-/sec21/blob/master/include/sec21/fixed_string.h)*

Compile-time string. Pretty useful as template parameter. 

> Possible will be replaced by the C++ standard
> [P3094R6](https://www.open-std.org/JTC1/SC22/WG21/docs/papers/2025/p3094r6.html)

### Example
```cpp
template <fixed_string Name, typename T>
struct anything { };

using foo_t = anything<"foo", int>;
```
