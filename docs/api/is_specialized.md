*#include [&lt;sec21/type_traits/is_specialized.h&gt;](https://github.com/MichaelMiller-/sec21/blob/master/include/sec21/type_traits/is_specialized.h)*

Checks if a specialisation of a template class is available.


### Example
```c++
namespace ns
{
   template <typename T>
   struct foo;

   template <>
   struct foo<int> {};

   template <>
   struct foo<std::string> {};
}

static_assert(is_specialized_v<ns::foo, int>);
static_assert(is_specialized_v<ns::foo, double> == false);
static_assert(is_specialized_v<ns::foo, float> == false);
static_assert(is_specialized_v<ns::foo, std::string> == true);
static_assert(is_specialized_v<ns::foo, std::wstring> == false);
```
