#include <catch.hpp>

#include <sec21/timestamp.h>

TEST_CASE("try to convert a string into timestamp", "[sec21][core]")
{
   using namespace sec21;

   REQUIRE(timestamp<long>("2001-01-01 23:53:23.777") == 978393203777);
}
