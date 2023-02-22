## split_if<span id="split_if"></span>

*#include [<sec21/split_if.h>](include/sec21/split_if.h)*

Divides the elements in a range, defined by [first, last), into two destination ranges depending on a predicate. If the predicate is true then the current element is appended to the first destination range, otherwise to the second.

### Example
```c++
std::vector in{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
std::vector<int> result1{};
std::vector<int> result2{};

split_if(begin(in), end(in), back_inserter(result1), back_inserter(result2), [](auto i) { return i < 5; });

// result1  == { 1, 2, 3, 4 }; 
// result2  == { 5, 6, 7, 8, 9, 10 }; 
```

