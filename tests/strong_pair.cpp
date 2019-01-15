#define _SILENCE_CXX17_UNCAUGHT_EXCEPTION_DEPRECATION_WARNING
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <sec21/strong_pair.h>

using sample_t = sec21::strong_pair<int, int>;

constexpr sample_t example00{ 0, 0 };
constexpr sample_t example01{ 0, 1 };
constexpr sample_t example10{ 1, 0 };
constexpr sample_t example11{ 1, 1 };

static_assert(example00 == example00, "must be equal");
static_assert(example01 == example01, "must be equal");
static_assert(example10 == example10, "must be equal");
static_assert(example11 == example11, "must be equal");

static_assert((example00 == example01) == false, "should be false");
static_assert((example10 == example11) == false, "should be false");

TEST_CASE("strong pair", "[compilation-only]") {
   SUCCEED("Nothing to test. Compiletests");
}
