## arity<span id="blob"></span>

*#include [<sec21/blob.h>](include/sec21/blob.h)*

Store any given type into a memory blob. 

### Example
```c++
struct foo
{
   int x{0};
   int y{0};
   double d{};
   float f{};
   std::array<int, 8> a{};
};

const foo obj{17, 42, 3.14, 4.2f, {1, 2, 4, 5, 6, 7, 8, 9}};

const auto blob = blob{obj};
REQUIRE(blob.size() == sizeof(foo));
```