*#include [&lt;sec21/validate.h&gt;](https://github.com/MichaelMiller-/sec21/blob/master/include/sec21/validate.h)*

### Every input has to be validated

Usually if it comes to input processing we tend to write something like that:
```c++
std::string any_input_string{"..."};

if (any_input_string == "list_files")
{
  call_list_files();
}
if (any_input_string.starts_with("input_file"))
{
  auto parts = split(any_input_string, "=");
  if (parts.size() == 2 && parts[0] == "input_file" && !parts[1].empty()) {
    process_input_file(parts[1]);
  }
  throw std::invalid_argument("cannot parse 'input_file' argument");
}
// ...
else {
  throw std::invalid_argument("invalid input");
}
```
This code is really tedious and very error prune and worst of all it is not very good testable. This is where **sec21::validate** comes in place.

It's [separate concerns](https://en.wikipedia.org/wiki/Separation_of_concerns) via small pieces of code that are not tied together, that makes it by default easy extensible. This design also makes it very to test, like the following example will show. 

### Testable

```c++
// define a action
struct action1 {};

// specialize the 'validate' template class
template <>
struct validate<action1>
{
   std::optional<action1> operator()(std::string_view input) const noexcept
   {
      if (input == "action1") {
         return action1{};
      }
      return std::nullopt;
   }
};

// write some test. in this case via Catch2
TEST_CASE("validate action1")
{
  SECTION("matches with input string")
  {
    validate<action1> obj;
    REQUIRE(obj("action1").has_value());
  }
  SECTION("misspelling 1")
  {
    validate<action1> obj;
    REQUIRE(obj("action21").has_value() == false);
  }
  SECTION("misspelling 2")
  {
    validate<action1> obj;
    REQUIRE(obj("action11").has_value() == false);
  }
}
```

### Example
```c++
// user actions
struct list_files {};
struct input_file { std::filesystem::path filename{}; };

// specialize the 'validate' template class
template <>
struct sec21::validate<list_files>
{
  std::optional<list_files> operator()(std::string_view input) const noexcept
  {
    if (input == "list_files") {
      return list_files{};
    }
    return std::nullopt;
  }
};

template <>
struct sec21::validate<input_file>
{
  std::optional<input_file> operator()(std::string_view input) const
  {
    std::vector<std::string> parts{};
    boost::split(parts, input, [](auto c) { return c == '='; });

    if (size(parts) == 2 && parts[0] == "input_file" && std::filesystem::exists(parts[1])) {
      return input_file{ parts[1] };
    }
    return std::nullopt;
  }
};

// validate any input. could be commandline-arguments, file-content, console, ...
auto result = input_dispatcher<list_files, input_file>{}(any_string);

// note: 'overloaded' class is found here: https://en.cppreference.com/w/cpp/utility/variant/visit
std::visit(overloaded{
        [](list_files){ std::cout << "list files action" << std::endl; },
        [](input_file const& obj){ std::cout << "input file= " << obj.filename << std::endl; },
        [](invalid_input){ std::cout << "invalid input" << std::endl; }
    }, result);
```

Be aware that std::variant is a type-safe union and the that any missing overload results into a compile time error. Therefore it is very hard to get things wrong.
