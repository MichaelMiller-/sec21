## transform<span id="transform"></span>

*#include [<sec21/transform.h>](include/sec21/transform.h)*

Transforms any given std::tuple<Ts...>.

```c++
auto input = std::make_tuple(1, 2, 3, 4.44);
auto result = transform(input, [](auto v) { return v * 2; });
   
// result is std::tuple<int, int, int, double>{ 2, 4, 6, 8.88 }; 
```

