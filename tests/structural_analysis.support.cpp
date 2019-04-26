#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <sec21/structural_analysis/support.h>

using namespace sec21::structural_analysis;

static_assert(reactions(support_kind::Free) == 0, "invalid reactions for free support");
static_assert(reactions(support_kind::Roller) == 1, "invalid reactions for roller support");
static_assert(reactions(support_kind::Hinge) == 2, "invalid reactions for hinged support");
static_assert(reactions(support_kind::Fixed) == 3, "invalid reactions for fixed support");
