## concat<span id="concat"></span>

*#include [<sec21/concat.h>](include/sec21/concat.h)*

Concat all elements from a std::tuple or a variadic template list, with a custom delimiter, to a std::string. 

### Example
```c++
auto input = std::make_tuple("foo", "bar", "baz");
auto result = concat(input, "+++"); 
// result == "foo+++bar+++baz"
```
