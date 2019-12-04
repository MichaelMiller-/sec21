#define CATCH_CONFIG_MAIN
#include "catch.hpp"

//! \todo P0887: identity metafunction
// C++ 20 check, macro, test
//template <typename T>
//struct type_identity
//{
//   using type = T;
//};
//template <typename T>
//using type_identity_t = typename type_identity<T>::type;

#include <sec21/units.h>

#include <limits>

TEST_CASE("quantity cast", "[units-v2]")
{
   using namespace sec21::units;

   static_assert(quantity_cast<quantity<meter, int>>(2_km).value() == 2'000);
   static_assert(quantity_cast<quantity<kilometer, int>>(2'000_m).value() == 2);

   SUCCEED("Compiletime-Tests");
}

TEST_CASE("quantity test", "[units-v2]")
{
   using namespace sec21::units;
   using example_unit = second;

   static_assert(std::is_same_v<quantity<example_unit, int>::value_t, int>);
   static_assert(std::is_same_v<quantity<example_unit, double>::value_t, double>);
   static_assert(std::is_same_v<quantity<example_unit, float>::value_t, float>);

   static_assert(std::is_same_v<quantity<second, int>::unit_t, second>);
   static_assert(std::is_same_v<quantity<minute, int>::unit_t, minute>);
   static_assert(std::is_same_v<quantity<hour, int>::unit_t, hour>);

   // constructors
   //
   static_assert(quantity<example_unit, int>().value() == 0);
   static_assert(quantity<example_unit, int>(512).value() == 512);

   constexpr quantity<second, int> one_hour{ 3600 };
   static_assert(one_hour.value() == 3600);
   static_assert(quantity<hour, int>(one_hour).value() == 1);

   static_assert(quantity<example_unit, double>(1).value() == 1.0);
   static_assert(quantity<example_unit, double>(3.14).value() == 3.14);

   //! \todo
   // assignment operator

   // static member functions
   //! \todo
   // static_assert(quantity<example_unit, int>::zero().value() == 0);
   static_assert(quantity<example_unit, int>::min().value() == std::numeric_limits<int>::lowest());
   static_assert(quantity<example_unit, int>::max().value() == std::numeric_limits<int>::max());

   //! \todo
   // static_assert(quantity<example_unit, double>::zero().value() == 0.0);

   static_assert(quantity<example_unit, double>::min().value() == std::numeric_limits<double>::lowest());
   static_assert(quantity<example_unit, double>::max().value() == std::numeric_limits<double>::max());

   // unary operators
   static_assert(-quantity<example_unit, int>{1} == quantity<example_unit, int>{-1});
   static_assert(-quantity<example_unit, int>{-1} == quantity<example_unit, int>{1});

   //! \todo
   // binary member operators

   //! \todo
   // compound assignment

   //! \todo
   // non-member arithmetic operators

   // comparators
   static_assert(2_m + 1_m == 3_m);
   static_assert(!(2_m + 2_m == 3_m));
   static_assert(2_m + 2_m != 3_m);
   static_assert(!(2_m + 2_m != 4_m));
   static_assert(2_m > 1_m);
   static_assert(!(1_m > 1_m));
   static_assert(1_m < 2_m);
   static_assert(!(2_m < 2_m));
   static_assert(2_m >= 1_m);
   static_assert(2_m >= 2_m);
   static_assert(!(2_m >= 3_m));
   static_assert(1_m <= 2_m);
   static_assert(2_m <= 2_m);
   static_assert(!(3_m <= 2_m));

   static_assert(1'000_m == 1_km);
   static_assert(1'001_m != 1_km);
   static_assert(999_m < 1_km);
   static_assert(1'000_m >= 1_km);
   static_assert(1'000_m <= 1_km);

   SUCCEED("Compiletime-Tests");
}

TEST_CASE("SI units test", "[units-v2]")
{
   using namespace sec21::units;

   // SI base dimensions
   //
   // time
   static_assert(3600_s == 3600_s);
   static_assert(1_h == 3600_s);

   // length
   static_assert(1_m == 100_cm);
   static_assert(1_m == 1'000_mm);
   static_assert(1_km == 1'000'000_mm);
   static_assert(1_km == 1'000_m);
   static_assert(1_km + 1_m == 1'001_m);
   // static_assert(10_km / 5_km == 2);
   static_assert(10_km / 2 == 5_km);
   // static_assert(100_mm / 5_cm == 2);

   // mass
   static_assert(1_kg == 1'000_g);
   static_assert(1_t == 1'000_kg);
   static_assert(1_t == 1'000'000_g);

   // frequency
   static_assert(2 / 1_s == 2_Hz);
   static_assert(120 / 1_min == 2_Hz);
   static_assert(1000 / 1_s == 1_kHz);
   static_assert(1 / 1_ms == 1_kHz);
   // static_assert(3.2_GHz == 3'200'000'000_Hz);
   // static_assert(10_Hz * 1_min == 600);

   // force
   static_assert(10_kg * 10_mps_sq == 100_N);
   static_assert(1_kN == 1'000_N);
   static_assert(1_MN == 1'000'000_N);

   // pressure
   static_assert(10_N / 10_sq_m == 1_Pa);

   // energy
   static_assert(10_N * 10_m == 100_J);
   // static_assert(10Pa * 10cub_m == 100_J);

   // power
   static_assert(10_J / 10_s == 1_W);

   // electric charge
   static_assert(10_A * 10_s == 100_C);

   // voltage
   // static_assert(10_W / 10_A == 1_V);
   // static_assert(10_J / 10_C == 1_V);

   // capacitance
   // static_assert(10_C / 10_V == 1_F);

   // velocity
   static_assert(10_m / 5_s == 2_mps);
   static_assert(1_km / 1_s == 1000_mps);
   static_assert(10 / 5_s * 1_m == 2_mps);
   //static_assert(2_kmph * 2_h == 4_km);
   //static_assert(2_km / 2_kmph == 1_h);
   static_assert(1_km / 1_s == 1000_mps);
   //static_assert(1.0km / 1h == 1kmph);
   //static_assert(1000.0m / 3600.0s == 1kmph);

   // acceleration
   static_assert(10_mps / 10_s == 1_mps_sq);

   // area
   static_assert(1_m * 1_m == 1_sq_m);
   // static_assert(10_km * 10_km == 100_sq_km);
   //static_assert(1sq_m == 10'000sq_cm);

   // volume
   static_assert(1_m * 1_m * 1_m == 1_cub_m);
   static_assert(10_sq_m * 10_m == 100_cub_m);
   //static_assert(10_km * 10_km * 10_km == 1000_cub_km);
   //static_assert(1_cub_m == 1'000'000_cub_cm);

   SUCCEED("Compiletime-Tests");
}

TEST_CASE("constant quantities", "[units-v2]")
{
   using namespace sec21::units::constants;
   using namespace sec21::units::literals;

   static_assert(G == 9.80665_mps_sq);

   SUCCEED("Compiletime-Tests");
}
