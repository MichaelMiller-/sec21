#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <sec21/structural_analysis/axis.h>

using namespace sec21::structural_analysis;

static_assert(static_cast<int>(Axis::X) == 0, "Value of X-Axis has to be 0");
static_assert(static_cast<int>(Axis::Y) == 1, "Value of Y-Axis has to be 1");
static_assert(static_cast<int>(Axis::Z) == 2, "Value of Z-Axis has to be 2");
