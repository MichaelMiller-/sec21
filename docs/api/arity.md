## arity <span id="arity"></span>

*#include [<sec21/arity.h>](include/sec21/arity.h)*

Count the number of arguments of any given callable, including generic lambdas and generic function objects.

### Example
```c++
// plan function
void func(int, int);
static_assert(arity(func) == 2);

// function object
struct binary_functor {
   template <typename T, typename U>
   void operator()(T, U) {}
};
static_assert(arity(binary_functor{}) == 2);

// generic lambda
const auto l3 = [](auto, auto, auto) {};
static_assert(arity(l3) == 3);
```