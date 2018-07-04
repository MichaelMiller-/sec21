#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "sec21/in_range.h"

//
// compiletime tests
//
// test with ints
static_assert(sec21::in_range(7, 1, 10));
static_assert(sec21::in_range(1, 1, 10));
static_assert(sec21::in_range(10, 1, 10));

static_assert(!sec21::in_range(-7, 1, 10));
static_assert(!sec21::in_range(-1, 1, 10));
static_assert(!sec21::in_range(11, 1, 10));

// test with doubles
static_assert(sec21::in_range(7.0, 1.0, 10.0));
static_assert(sec21::in_range(1.0, 1.0, 10.0));
static_assert(sec21::in_range(10.0, 1.0, 10.0));

static_assert(!sec21::in_range(-7.0, 1.0, 10.0));
static_assert(!sec21::in_range(-1.0, 1.0, 10.0));
static_assert(!sec21::in_range(11.0, 1.0, 10.0));
