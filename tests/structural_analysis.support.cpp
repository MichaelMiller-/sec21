#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <sec21/structural_analysis/support.h>

using namespace sec21::structural_analysis;

static_assert(static_cast<int>(Support::Free) == 0);
static_assert(static_cast<int>(Support::Roller) == 1);
static_assert(static_cast<int>(Support::Hinge) == 2);
static_assert(static_cast<int>(Support::Fixed) == 3);

static_assert(variability(Support::Free) == 0);
static_assert(variability(Support::Roller) == 1);
static_assert(variability(Support::Hinge) == 2);
static_assert(variability(Support::Fixed) == 3);

TEST_CASE("combine", "[compilation-only]") {
   SUCCEED("Nothing to test. Compiletests");
}

