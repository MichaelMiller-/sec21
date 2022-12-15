## all_of / any_of <span id="allanyof"></span>

*#include [<sec21/all_of.h>](include/sec21/all_of.h)* <br/>
*#include [<sec21/any_of.h>](include/sec21/any_of.h)*

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