*#include [&lt;sec21/meta/difference.h&gt;](https://github.com/MichaelMiller-/sec21/blob/master/include/sec21/meta/difference.h)*

Return the difference of two type-lists. An optional compartor can also be defined.

### Example
```c++
{
    using T1 = type_list<int, float, char>;
    using T2 = type_list<double, float, short, int>;
    using R1 = difference<T1, T2>::type;
    static_assert(std::is_same_v<R1, type_list<char>>);
}
{
    using T1 = std::tuple<int, char>;
    using T2 = std::tuple<double, float, short, int>;
    using R1 = difference<T1, T2>::type;
    static_assert(std::is_same_v<R1, std::tuple<char>>);
}
```
