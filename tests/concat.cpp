#include <catch.hpp>

#include <sec21/concat.h>

struct foo
{
   int x{0};
   int y{0};
};

// needed for catch output formatter
template <typename CharT, typename Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, foo const& f)
{
   return os << '(' << f.x << ", " << f.y << ")";
}

TEST_CASE("concat string-convertible objects", "[sec21][core]")
{
   foo obj{17, 42};
   const auto result = sec21::concat<char>("string1", 4, "foo", 3.14, obj, "bar");
   const std::string expected{"string14foo3.14(17, 42)bar"};

   REQUIRE(result == expected);
}

TEST_CASE("concat a tuple with a explicit delimiter and a transform operation") {

   const auto input = std::make_tuple(1, 2, 3, 4);
   REQUIRE(sec21::concat(input, ",", [](auto v){ return v * v; }) == "1,4,9,16");
}

TEST_CASE("concat a tuple with a explicit delimiter")
{
   const auto input = std::make_tuple("foo", "bar", "baz");
   REQUIRE(sec21::concat(input, "+++") == "foo+++bar+++baz");
}