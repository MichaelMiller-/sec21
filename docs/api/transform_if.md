*#include [&lt;sec21/transform_if.h&gt;](https://github.com/MichaelMiller-/sec21/blob/master/include/sec21/transform_if.h)*

Transforms any element in a range, defined by [first, last), into a destination range if the predicate is satisfied. 

### Example
```c++
std::vector in{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
std::vector<Data> result{};

transform_if(
  std::begin(in),
  std::end(in),
  std::back_inserter(result),
  [](auto i) { return i < 5; },
  [](auto i) { return Data{i}; });

// result == { Data{1}, Data{2}, Data{3}, Data{4} }; 
```
