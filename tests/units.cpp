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

//! \todo conversion error -> mismatch between int and double
// static_assert(1_g == 0.001_kg, "must be equal");

static_assert(1.0_kN == 1000.0_N, "must be equal");
static_assert(1.0_MN == 1000.0_kN, "must be equal");
static_assert(1.0_MN == 1'000'000.0_N, "must be equal");

static_assert(1_h == 3'600_s, "must be equal");

// simple numeric operations
// add
static_assert(1_m + 1_m == 2_m, "must be equal");
static_assert(1_km + 1_m == 1001_m, "must be equal");
static_assert(5_in + 8_cm == 207_mm, "must be equal");

// substract
static_assert(2_m - 1_m == 1_m, "must be equal");

// multiply
//static_assert(10_km / 2 == 5_km);
// static_assert(1000 / 1_s == 1_kHz);
// static_assert(10_km / 5_km == 2);

// negation
static_assert(-1.0_MN == -1.0_MN, "must be equal");


// dimension conversions
static_assert(1.0_Pa == (1.0_N / (1.0_m * 1.0_m)), "must be equal");

//static_assert(1_km / 1_s == 1000_mps, "must be equal");
//static_assert(2_kmph * 2_h == 4_km, "must be equal");
//static_assert(4_km / 2_kmph == 2_h, "must be equal");
//static_assert(1000 / 1_s == 1_kHz, "must be equal");
//static_assert(10_km / 5_km == 2, "must be equal");


#include <boost/version.hpp>

#if BOOST_VERSION == 106700
//! \todo fix: boost::mpl::transform didn't consider the order of the sequences in previos version
//! \todo fix: check boost version or write a workaround 
static_assert(1_m * 1_m == unit::square_meter<int>(1), "must be equal");
static_assert(3_ft * 4_ft == unit::square_foot<int>{12}, "must be equal");

static_assert((1_m * 1_m * 1_m) == unit::cubic_meter<int>(1), "must be equal");

static_assert((1_m / 1_m) == unit::id_t<int>(1), "must be equal");

static_assert(15_kg * unit::constants::g == unit::newton<decltype(F)::value_t>(147.099750), "must be equal");

#endif

// simple interface test

//constexpr unit::quantity<velocity> average_speed(unit::quantity<length> d, unit::quantitya<time> t)
//{
//   return d / t;
//}

TEST_CASE("units", "[compilation-only]") 
{
   SUCCEED("Nothing to test. Compiletests");
}

#ifdef __cpp_concepts

//! test c++20 concepts
template <typename T>
auto len(T t) requires sec21::unit::Length<T> { return t.get(); }

TEST_CASE("runtime tests", "[units]")
{
   using namespace sec21::unit;
   REQUIRE(len(meter<double>(5.0)) == Approx(5.0));
   REQUIRE(len(kilometer<double>(5.0)) == Approx(5.0));
}

#endif

