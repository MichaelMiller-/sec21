## strong_type <span id="strongtype"></span>

*#include [<sec21/strong_type.h>](include/sec21/strong_type.h)*

The static typing of the programming language C ++ allows you to design types that exactly meet the specified requirements. Due to the [policy-based design](https://en.wikipedia.org/wiki/Modern_C%2B%2B_Design#Policy-based_design) of 'strong_type' it is very easy to design new types. For example, a counter-like type should be designed which can only be incremented. Trying to decrement an object of this type will result in a compiler error. 
```c++
using counter_t = strong_type<int, struct counter_tag, policy::increment>;

counter_t cnt{ 42 };
++cnt;          // counter_t.value == 43
// --cnt;       // is not possible
```
Due to the specific type, interfaces can be developed much more safely and clearly.
The following example illustrates this well. The interface uses POD data types as function arguments.
When the function is called, the order of the arguments cannot be guaranteed and errors can occur during execution. 
```c++
void set_window_dimension(int width, int height);
// ...
int input_width{ 800 };
int input_height{ 800 };

// interface call
void set_window_dimension(input_width, input_height);
// work's as well
void set_window_dimension(input_height, input_width);
```
A strong_type function argument, however, forces the caller side to explicitly specify the type. Among other things, this also improves the readability of the code. 
```c++
using width_t = strong_type<int, struct width_tag>;
using height_t = strong_type<int, struct height_tag>;

void set_window_dimension(width_t{ 800 }, height_t{ 600 });
```
Furthermore, the behavior and the interface of the strong_type can be extended and tested very easily. The following example shows a type that represents an alphabet. Incrementing above the last letter can throw an error or provide a new value with an index. 

```c++
template <typename T>
// A(0) ... Z(0) ... A1 ... Z1 ... A2 ... Z2 ... 
struct increment_alphabet
{
   T& operator ++ () noexcept
   {
      using type_t = typename T::underlying_t;
      static_assert(std::is_same_v<type_t, std::string>);

      // get reference to the underlaying type
      auto& result = static_cast<type_t&>(static_cast<T&>(*this));

      // modify value
      if (result[0] == 'Z') {
         const auto tmp = type_t{ next(begin(result)), end(result) };
         unsigned long long number{0};
         std::from_chars(tmp.data(), tmp.data() + tmp.size(), number);
         result = fmt::format("A{}", ++number);
      }
      else
         ++result[0];

      return static_cast<T&>(*this);
   }
};

using alhpabet_t = strong_type<
   std::string, 
   struct alhpabet_tag, 
   increment_alphabet, 
   policy::compare>;

alhpabet_t obj{ "Z" };
++obj;
REQUIRE(obj == "A1");
```
