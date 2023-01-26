## for_each_indexed<span id="for_each_indexed"></span>

*#include [<sec21/for_each_indexed.h>](include/sec21/for_each_indexed.h)*

Executes the passed function per element with the associated index.

```c++
auto input = std::vector{ 4, 3, 2, 1 };
for_each_indexed(
   begin(input), 
   end(input), 
   [](auto index, auto value) { std::cout << "i: " << index << " v: " << value << std::endl; });

// output:
// i: 0 v: 4  
// i: 1 v: 3  
// i: 2 v: 2  
// i: 3 v: 1  
```
