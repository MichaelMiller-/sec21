#define _SILENCE_CXX17_UNCAUGHT_EXCEPTION_DEPRECATION_WARNING
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <sec21/units.h>

namespace unit = sec21::unit;
using namespace unit::literals;

// unit conversions
static_assert(1_m == 100_cm, "must be equal");
static_assert(1_km == 1'000'000_mm, "must be equal");
static_assert(1_km == 1000_m, "must be equal");
static_assert(1_ft == 12_in, "must be equal");

static_assert(1_kg == 1000_g, "must be equal");
static_assert(1_t == 1000_kg, "must be equal");
static_assert(1_t == 1'000'000_g, "must be equal");
static_assert(1.0_lb == 0.4535924_kg, "must be equal");
static_assert(1.0_g == 0.001_kg, "must be equal");

static_assert(1.0_kN == 1000.0_N, "must be equal");
static_assert(1.0_MN == 1000.0_kN, "must be equal");
static_assert(1.0_MN == 1'000'000.0_N, "must be equal");

static_assert(1_h == 3'600_s, "must be equal");

// simple numeric operations
// addition
static_assert(1_m + 1_m == 2_m, "must be equal");
static_assert(1_km + 1_m == 1001_m, "must be equal");
static_assert(5_in + 8_cm == 207_mm, "must be equal");
static_assert(1_m + 4 == 5_m, "must be equal");    // with scalar

//! \todo special for values from a quantity of time

// substract
static_assert(2_m - 1_m == 1_m, "must be equal");
static_assert(20_m - 5 == 15_m, "must be equal");
static_assert(1.0_kN - 5.0 == -4.0_kN, "must be equal");

// multiply
static_assert(10_km / 2 == 5_km);
// static_assert(1000 / 1_s == 1_kHz);

// negation
static_assert(-1.0_MN == -1.0_MN, "must be equal");

// dimension conversions
static_assert(1.0_Pa == (1.0_N / (1.0_m * 1.0_m)), "must be equal");

//static_assert(1_km / 1_s == 1000_mps, "must be equal");
//static_assert(2_kmph * 2_h == 4_km, "must be equal");
//static_assert(4_km / 2_kmph == 2_h, "must be equal");


static_assert(1_m * 1_m == unit::square_meter<int>(1), "must be equal");
static_assert(3_ft * 4_ft == unit::square_foot<int>{12}, "must be equal");

static_assert((1_m * 1_m * 1_m) == unit::cubic_meter<int>(1), "must be equal");

static_assert((1_m / 1_m) == unit::id_t<int>(1), "must be equal");

static_assert(15.0_kg * unit::constants::g == unit::newton<double>(147.099750), "must be equal");

TEST_CASE("units", "[compilation-only]") 
{
   SUCCEED("Nothing to test. Compiletests");
}
