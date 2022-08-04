## expects <span id="expects"></span>

*#include [<sec21/expects.h>](include/sec21/expects.h)*

A simple one-liner that provides general error handling. If the condition evaluates to false, a centrally defined action is executed with the passed text.

```cpp
// if the condition is false a std::runtime_error is thrown
expects([=]{ return value > 23; }, "value has to be greater than 23");
```

