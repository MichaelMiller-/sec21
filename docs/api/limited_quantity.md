*#include [<sec21/limited_quantity.h&gt;](https://github.com/MichaelMiller-/sec21/blob/master/include/sec21/limited_quantity.h)*

Represents a value between two fixed endpoints [Infimum, Supremum]. 


```c++
using value_t = sec21::limited_quantity<int, 1, 10>;

{
   value_t value{10};
   ++value; // throws a exception
}
{
   value_t value{1};
   --value; // throws a exception
}
```

