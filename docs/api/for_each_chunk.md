*#include [&lt;sec21/for_each_chunk.h&gt;](https://github.com/MichaelMiller-/sec21/blob/master/include/sec21/for_each_chunk.h)*

Apply a function to every chunk in a range.

```cpp
auto input = std::vector{ 1, 2, 3, 4, 5, 6, 7, 8 };
for_each_chunk<2>(input, [](auto x, auto y){ std::cout << "x: " << x << " y: " << y << std::endl; });

// output:
// x: 1 y: 2  
// x: 3 y: 4  
// x: 5 y: 6  
// x: 7 y: 8  
```

