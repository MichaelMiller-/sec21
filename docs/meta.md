# sec21::meta

All meta functions in this library work with any generic [list of types](api/meta.type_list.md). This includes standard types such as [std::tuple](https://en.cppreference.com/cpp/utility/tuple) and [std::variant](https://en.cppreference.com/cpp/utility/variant), as well as other types such as [dimension](https://github.com/MichaelMiller-/sec21/blob/master/include/sec21/units/dimension.h) from the units library.

```c++
template <typename... Exponents>
struct dimension
{
};
```

| Name (alphabetical order)                  | Description                                             |
|:-------------------------------------------|:--------------------------------------------------------|
| [contains](api/meta.contains.md)           | Check if a type-list contains a given type              |
| [difference](api/meta.difference.md)       | Return the difference of two type-lists                 |
| [intersection](api/meta.intersection.md)   | Return the intersection of two type-lists               | 
| [merge](api/meta.merge.md)                 | Merge several type-list into one list                   |
| [push_front](api/meta.push_front.md)       | Push a given type to the front of any list              |
| [remove_if](api/meta.remove_if.md)         | Remove a type that satisfy a predicate from a type-list |
| [sort](api/meta.sort.md)                   | Sort any given type-list                                |
| [type_list](api/meta.type_list.md)         | Generic type-list                                       |
| [zip_transform](api/meta.zip_transform.md) | Zip and transform two type-lists                        |

