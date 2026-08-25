*#include [&lt;sec21/bit_pattern.h&gt;](https://github.com/MichaelMiller-/sec21/blob/master/include/sec21/bit_pattern.h)*

Matches the bits via a pattern. The letter **x** represents a placeholder.

### Example
```c++
static_assert(0b1101010u == bit_pattern("11xxx10"));
```

