## to_array <span id="to_array"></span>

*#include [<sec21/to_array.h>](include/sec21/to_array.h)*

Transforms any given std::tuple<Ts...> into a std::array. 

```c++
auto input = std::make_tuple(1, 2, 3, 4.44f, 5.55, "string");
auto result = to_array(input, [](auto v) {
  std::stringstream ss;
  ss << "v: " << v;
  return ss.str();
});

// result is std::array<std::string, 6>{  "v: 1", "v: 2", "v: 3", "v: 4.44", "v: 5.55", "v: string" }; 
```

