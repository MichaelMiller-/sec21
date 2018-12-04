#define _SILENCE_CXX17_UNCAUGHT_EXCEPTION_DEPRECATION_WARNING
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <sec21/units.h>

namespace su = sec21::units;
using namespace su::literals;

constexpr su::Meter<int> m1{ 1 };
constexpr su::Meter<int> m2{ 2 };
constexpr su::Centimeter<int> m200{ 200 };

constexpr su::Foot<int> f1{ 1 };
constexpr su::Inch<int> i12{ 12 };

constexpr auto r1 = su::Inch<int>(5) + su::Centimeter<int>(8);
constexpr auto r2 = m1 * m1;

// equal tests
static_assert(m1 == m1, "must be equal");
static_assert(m2 == m2, "must be equal");
static_assert(m200 == m2, "must be equal");
static_assert(f1 == i12, "must be equal");

//! \todo more tests
// operations
static_assert(m1 + m1 == m2, "must be equal");
static_assert(r1 == 207_mm, "must be equal");

static_assert(m2 - m1 == m1, "must be equal");

#include <boost/version.hpp>

#if BOOST_VERSION >= 106700

static_assert(r2 == su::SquareMeter<int>(1), "must be equal");
static_assert((su::Foot<int>{3} * su::Foot<int>{4}) == su::SquareFoot<int>{12}, "must be equal");

static_assert((m1 * m1 * m1) == su::CubicMeter<int>(1), "must be equal");

#endif

static_assert((m1 / m1) == su::Id<int>(1), "must be equal");

// unit conversion
static_assert(1_m == 100_cm, "must be equal");
static_assert(1_km == 1'000'000_mm, "must be equal");
static_assert(1_km == 1000_m, "must be equal");

static_assert(1_kg == 1000_g, "must be equal");
static_assert(1_t == 1000_kg, "must be equal");
static_assert(1_t == 1'000'000_g, "must be equal");
//! \todo conversion error -> mismatch between int and double
//static_assert(1_g == 0.001_kg, "must be equal");

static_assert(1.0_kN == 1000.0_N, "must be equal");
static_assert(1.0_MN == 1000.0_kN, "must be equal");
static_assert(1.0_MN == 1'000'000.0_N, "must be equal");


//TEST_CASE("runtime tests", "[units]")
//{
//   bool b = false;
//   REQUIRE(b);
//}
