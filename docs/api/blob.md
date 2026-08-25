*#include [&lt;sec21/blob.h&gt;](https://github.com/MichaelMiller-/sec21/blob/master/include/sec21/blob.h)*

This class represents any type as a [memory blob](https://en.wikipedia.org/wiki/Object_storage).   

### Example
```c++
struct foo
{
   int x{0};
   int y{0};
   double d{};
   float f{};
   std::array<int, 8> a{};
};

foo obj{17, 42, 3.14, 4.2f, {1, 2, 4, 5, 6, 7, 8, 9}};
// store object in a blob
auto mem = blob{obj};
// restore object of type 'foo' from the memory blob
foo result = mem;
// or simply retrieve the first integer
int i = mem;  // i == 17

// It is also possible to convert the memory blob into a std::string
std::string str = to_string(mem);
// and read from a std::string
blob mem1{blob::parse, str};
```

