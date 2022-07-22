#include <catch.hpp>

// either
// * returns a new function with N parameters bound
// * executes the function
template <typename Callable, typename... Args>
auto curry(Callable f, Args... args)
{
   if constexpr (requires { f(args...); }) {
      return f(args...);
   } else {
      return [f, args...](auto ... inner_args)
      {
         return curry(f, args..., inner_args...);
      };
   }
}

int add(int x, int y, int z) { return x + y + z; }

TEST_CASE("curry", "[sec21]")
{
   auto new_func = curry(add, 1, 2);
   auto result = new_func(3);

   REQUIRE(result == 6);
}