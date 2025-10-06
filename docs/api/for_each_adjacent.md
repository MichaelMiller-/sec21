*#include [&lt;sec21/for_each_adjacent.h&gt;](https://github.com/MichaelMiller-/sec21/blob/master/include/sec21/for_each_adjacent.h)*

Apply a function for two consecutive elements in a sequence.

```cpp
auto input = std::vector{ 1, 2, 3, 4, 5, 6, 7, 8 };
for_each_adjacent(input, [](auto x, auto y){ std::cout << "x: " << x << " y: " << y << std::endl; });

// output:
// x: 1 y: 2  
// x: 2 y: 3  
// x: 3 y: 4
// x: 4 y: 5
// x: 5 y: 6
// x: 6 y: 7
// x: 7 y: 8
```
