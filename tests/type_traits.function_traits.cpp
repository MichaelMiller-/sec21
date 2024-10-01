#include <catch.hpp>

#include <sec21/type_traits/function_traits.h>

int f(float, const char*);

TEST_CASE("decompose a function", "[sec21][type_traits]")
{
   using namespace sec21::type_traits;

   using traits = function_traits<decltype(f)>;

   STATIC_REQUIRE(std::is_same_v<int, traits::result_type>);
   STATIC_REQUIRE(std::is_same_v<float, traits::argument_type<0>>);
   STATIC_REQUIRE(std::is_same_v<const char*, traits::argument_type<1>>);
   STATIC_REQUIRE(traits::arity == 2);
}
