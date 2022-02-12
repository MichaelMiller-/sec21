#include "catch.hpp"

#include <sec21/enum_to_string.h>

enum class DaysOfTheWeek { Monday, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday };

using namespace sec21;

TEST_CASE("convert enum to string", "[sec21][core]")
{
#if HAS_REFLECTION
   std::string s = to_string(DaysOfTheWeek::Friday);
   REQUIRE(s == "Friday");
#else
   WARN("c++ reflection header is not found");
   REQUIRE(true);
#endif
}

TEST_CASE("convert string to enum", "[sec21][core]")
{
#if HAS_REFLECTION
   const auto v = to_enum<DaysOfTheWeek>("sunday");
   REQUIRE(v == DaysOfTheWeek::Sunday);
#else
   WARN("c++ reflection header is not found");
   REQUIRE(true);
#endif
}