*#include [&lt;sec21/zip.h&gt;](https://github.com/MichaelMiller-/sec21/blob/master/include/sec21/zip.h)*

Iterate over several containers at the same time and call a function with the respective contents per iteration step.

### Example
```c++
std::vector keys{ 1, 2, 3 };
std::vector<std::string> values{ "foo", "bar", "baz" };
std::map<int, std::string> result;

zip(
   [&result](auto key, auto value) { result[key] = value; },
   begin(keys),
   end(keys),
   begin(values));

// result = { { 1, "foo" }, { 2, "bar" }, { 3, "baz" } }
```
Or reorder the content from a container and store it in a new one.

### Example
```c++
std::vector in{1,3,5,7,2,4,6,8};
std::vector<int> result{};

auto it1 = begin(in);
auto it2 = next(begin(in), 4);

zip([&result](auto v1, auto v2){
    result.push_back(v1);
    result.push_back(v2);
}, it1, it2, it2);

// result == { 1, 2, 3, 4, 5, 7, 8 };
```
