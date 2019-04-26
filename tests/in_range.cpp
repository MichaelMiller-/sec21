#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <sec21/in_range.h>

//
// compiletime tests
//
// test with ints
static_assert(sec21::in_range(7, 1, 10), "7 should be in range of 1 to 10");
static_assert(sec21::in_range(1, 1, 10), "1 should be in range of 1 to 10");
static_assert(sec21::in_range(10, 1, 10), "10 should be in range of 1 to 10");

static_assert(!sec21::in_range(-7, 1, 10), "-7 shouldn't be in range of 1 to 10");
static_assert(!sec21::in_range(-1, 1, 10), "-1 shouldn't be in range of 1 to 10");
static_assert(!sec21::in_range(11, 1, 10), "11 shouldn't be in range of 1 to 10");

// test with doubles
static_assert(sec21::in_range(7.0, 1.0, 10.0), "7.0 should be in range of 1.0 to 10.0");
static_assert(sec21::in_range(1.0, 1.0, 10.0), "1.0 should be in range of 1.0 to 10.0");
static_assert(sec21::in_range(10.0, 1.0, 10.0), "10.0 should be in range of 1.0 to 10.0");

static_assert(!sec21::in_range(-7.0, 1.0, 10.0), "-7.0 shouldn't be in range of 1.0 to 10.0");
static_assert(!sec21::in_range(-1.0, 1.0, 10.0), "-1.0 shouldn't be in range of 1.0 to 10.0");
static_assert(!sec21::in_range(11.0, 1.0, 10.0), "11.0 shouldn't be in range of 1.0 to 10.0");

TEST_CASE("in_range", "[compilation-only]") 
{
   SUCCEED("Nothing to test. Compiletests");
}
