*#include [&lt;sec21/all_of.h&gt;](https://github.com/MichaelMiller-/sec21/blob/master/include/sec21/all_of.h)* <br/>
*#include [&lt;sec21/any_of.h&gt;](https://github.com/MichaelMiller-/sec21/blob/master/include/sec21/any_of.h)*

Zero overhead syntax helper which also works with lambdas.

### Example
```c++
int x{10};
int y{50};
int z{99};

if (all_of{x, y, z} < 100) {
   // do something
}

auto lambda1 = [](auto e){ return e < 100; };
auto lambda2 = [](auto e){ return e > 5; };
auto lambda3 = [](auto e){ return e > 10; };

auto func = all_of{lambda1, lambda2, lambda3};

if (func(16)) {
   // do something
}
```

Those classes could also be used in a compile-time context.
### Example
```c++
static_assert(all_of{10, 50, 99} < 100); 
```
