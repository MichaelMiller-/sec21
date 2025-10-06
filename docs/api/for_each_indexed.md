*#include [&lt;sec21/for_each_indexed.h&gt;](https://github.com/MichaelMiller-/sec21/blob/master/include/sec21/for_each_indexed.h)*

Executes the passed function per element with the associated index. There is another overload where the start index can be specified.

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

## Alternatives
The C++ standard also offers alternatives.

### STL with a mutable lambda 
```c++
auto input = std::vector{ 4, 3, 2, 1 };
std::for_each(
   begin(input), 
   end(input), 
   [index = 0](auto value) mutable { std::cout << "i: " << index++ << " v: " << value << std::endl; });
```

### C++20 ranges
```c++
auto input = std::vector{ 4, 3, 2, 1 };
for (auto const [index, value] : std::views::enumerate(input))
{
   std::cout << "i: " << index << " v: " << value << std::endl;
}
```