## sec21::type_traits <span id="units"></span>

Templates for fundamental properties of types.


----
## has_type

*#include [<sec21/type_traits/has_type.h>](include/sec21/type_traits/has_type.h)*

Template which checks whether an explicit type is in a type list. There are also specialisations for std::tuple and std::variant.

### Example
```c++
using type_list_t = std::tuple<int, float, double>;

static_assert(has_type<int, type_list_t>::value == true);
static_assert(has_type<float, type_list_t>::value == true);
static_assert(has_type<double, type_list_t>::value == true);
static_assert(has_type<char, type_list_t>::value == false);
```

----
## is_specialized

*#include [<sec21/type_traits/is_specialized.h>](include/sec21/type_traits/is_specialized.h)*

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

---
## is_std_string

*#include [<sec21/type_traits/is_std_string.h>](include/sec21/type_traits/is_std_string.h)*

Checks whether the type is a std::string.

### Example
```c++
static_assert(is_std_string<std::string>::value == true);
static_assert(is_std_string<int>::value == false);
static_assert(is_std_string<std::wstring>::value == false);
static_assert(is_std_string<float>::value == false);
```
