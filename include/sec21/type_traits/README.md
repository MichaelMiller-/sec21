## sec21::type_traits

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
## index_of

*#include [<sec21/type_traits/index_of.h>](include/sec21/type_traits/index_of.h)*

Calculates the index of a type in a type list.

### Example
```c++
static_assert(index_of<int, int, float, char>::value == 0);
static_assert(index_of<float, int, float, char>::value == 1);
static_assert(index_of<char, int, float, char>::value == 2);
static_assert(index_of<double, int, float, char>::value == -1);   // -1 == not found
```

----
## instance_of

*#include [<sec21/type_traits/instance_of.h>](include/sec21/type_traits/instance_of.h)*

Checks whether a type corresponds to an instantiation of a class.

### Example
```c++
static_assert(is_instance_of_v<std::basic_string, std::string> == true);
static_assert(is_instance_of_v<std::tuple, std::tuple<int, double>> == true);
static_assert(is_instance_of_v<std::tuple, std::vector<int>> == false);
static_assert(is_instance_of_v<std::vector, std::tuple<int, double>> == false);
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


---
## is_tuple

*#include [<sec21/type_traits/is_tuple.h>](include/sec21/type_traits/is_tuple.h)*

Checks whether the type is a std::tuple<Ts...>.

### Example
```c++
static_assert(is_tuple_v<std::tuple<int, float, int>> == true);
static_assert(is_tuple_v<std::tuple<>> == true);
static_assert(is_tuple_v<int> == false);
static_assert(is_tuple_v<std::string> == false);
static_assert(is_tuple_v<double> == false);
```

---
## member_pointer_traits

*#include [<sec21/type_traits/member_pointer_traits.h>](include/sec21/type_traits/member_pointer_traits.h)*

Decomposes a member pointer into its components.

### Example
```c++
class cls {};

static_assert(std::is_same_v<member_pointer_traits<int(cls::*)>::class_t, cls>);
static_assert(std::is_same_v<member_pointer_traits<int(cls::*)>::value_t, int>);
```
