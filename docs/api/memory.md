*#include [<sec21/memory.h&gt;](https://github.com/MichaelMiller-/sec21/blob/master/include/sec21/memory.h)*

A small helper class to deal with memory units. Including arithmetic operations, string literals and specialization for std::format.

### Example
```c++
#include <sec21/literals/memory.h>
using namespace sec21::literals;

// arithmetic operations
auto value1 = 4_kiB + std::byte{4}; // memory{4100}

// :h shows the value in a human-readable format
auto value2 = std::format("{:h}", memory{1_kiB}); // "1.02kB"
auto value3 = std::format("{:.0h}", memory{1024 * 1024}); // == "1MB"
```

