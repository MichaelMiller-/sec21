#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "sec21/concat.h"

struct foo
{
   int x{ 0 };
   int y{ 0 };
};

#include <ostream>

template <typename CharT, typename Traits>
std::basic_ostream<CharT, Traits>& operator << (std::basic_ostream<CharT, Traits>& os, foo const& f) {
   return os << '(' << f.x << ", " << f.y << ")";
}

TEST_CASE("concat string-convertiable objects", "[string]")
{
   using namespace sec21;

   foo obj{ 17, 42 };
   const auto result = concat<char>("string1", 4, "foo", 3.14, obj, "bar");

   const std::string expected{ "string14foo3.14(17, 42)bar" };

   REQUIRE(result == expected);
}
