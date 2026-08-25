*#include [<sec21/type_traits/member_pointer_traits.h&gt;](https://github.com/MichaelMiller-/sec21/blob/master/include/sec21/type_traits/member_pointer_traits.h)*

Decomposes a pointer to a member into it's components.

### Example
```c++
class cls {};

static_assert(std::is_same_v<member_pointer_traits<int(cls::*)>::class_t, cls>);
static_assert(std::is_same_v<member_pointer_traits<int(cls::*)>::value_t, int>);
```
