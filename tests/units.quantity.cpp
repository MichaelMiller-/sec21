#define CATCH_CONFIG_MAIN
#include <catch.hpp>

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

TEST_CASE("quantity cast", "[sec21][units]")
{
   using namespace sec21::units;

   STATIC_REQUIRE(quantity_cast<quantity<meter, int>>(2_km).value() == 2'000);
   STATIC_REQUIRE(quantity_cast<quantity<kilometer, int>>(2'000_m).value() == 2);
}

TEST_CASE("quantity test", "[sec21][units]")
{
   using namespace sec21::units;
   using example_unit = second;

   STATIC_REQUIRE(std::is_same_v<quantity<example_unit, int>::value_t, int>);
   STATIC_REQUIRE(std::is_same_v<quantity<example_unit, double>::value_t, double>);
   STATIC_REQUIRE(std::is_same_v<quantity<example_unit, float>::value_t, float>);

   STATIC_REQUIRE(std::is_same_v<quantity<second, int>::unit_t, second>);
   STATIC_REQUIRE(std::is_same_v<quantity<minute, int>::unit_t, minute>);
   STATIC_REQUIRE(std::is_same_v<quantity<hour, int>::unit_t, hour>);

   // constructors
   //
   STATIC_REQUIRE(quantity<example_unit, int>().value() == 0);
   STATIC_REQUIRE(quantity<example_unit, int>(512).value() == 512);

   constexpr quantity<second, int> one_hour{ 3600 };
   STATIC_REQUIRE(one_hour.value() == 3600);
   STATIC_REQUIRE(quantity<hour, int>(one_hour).value() == 1);

   STATIC_REQUIRE(quantity<example_unit, double>(1).value() == 1.0);
   STATIC_REQUIRE(quantity<example_unit, double>(3.14).value() == 3.14);

   //! \todo
   // assignment operator

   // static member functions
   //! \todo
   // STATIC_REQUIRE(quantity<example_unit, int>::zero().value() == 0);
   STATIC_REQUIRE(quantity<example_unit, int>::min().value() == std::numeric_limits<int>::lowest());
   STATIC_REQUIRE(quantity<example_unit, int>::max().value() == std::numeric_limits<int>::max());

   //! \todo
   // STATIC_REQUIRE(quantity<example_unit, double>::zero().value() == 0.0);

   STATIC_REQUIRE(quantity<example_unit, double>::min().value() == std::numeric_limits<double>::lowest());
   STATIC_REQUIRE(quantity<example_unit, double>::max().value() == std::numeric_limits<double>::max());

   // unary operators
   STATIC_REQUIRE(-quantity<example_unit, int>{1} == quantity<example_unit, int>{-1});
   STATIC_REQUIRE(-quantity<example_unit, int>{-1} == quantity<example_unit, int>{1});

   //! \todo
   // binary member operators

   //! \todo
   // compound assignment

   //! \todo
   // non-member arithmetic operators

   // comparators
   STATIC_REQUIRE(2_m + 1_m == 3_m);
   STATIC_REQUIRE(!(2_m + 2_m == 3_m));
   STATIC_REQUIRE(2_m + 2_m != 3_m);
   STATIC_REQUIRE(!(2_m + 2_m != 4_m));
   STATIC_REQUIRE(2_m > 1_m);
   STATIC_REQUIRE(!(1_m > 1_m));
   STATIC_REQUIRE(1_m < 2_m);
   STATIC_REQUIRE(!(2_m < 2_m));
   STATIC_REQUIRE(2_m >= 1_m);
   STATIC_REQUIRE(2_m >= 2_m);
   STATIC_REQUIRE(!(2_m >= 3_m));
   STATIC_REQUIRE(1_m <= 2_m);
   STATIC_REQUIRE(2_m <= 2_m);
   STATIC_REQUIRE(!(3_m <= 2_m));

   STATIC_REQUIRE(1'000_m == 1_km);
   STATIC_REQUIRE(1'001_m != 1_km);
   STATIC_REQUIRE(999_m < 1_km);
   STATIC_REQUIRE(1'000_m >= 1_km);
   STATIC_REQUIRE(1'000_m <= 1_km);
}

TEST_CASE("SI units test", "[units]")
{
   using namespace sec21::units;

   // SI base dimensions
   //
   // time
   STATIC_REQUIRE(3600_s == 3600_s);
   STATIC_REQUIRE(1_h == 3600_s);

   // length
   STATIC_REQUIRE(1_m == 100_cm);
   STATIC_REQUIRE(1_m == 1'000_mm);
   STATIC_REQUIRE(1_km == 1'000'000_mm);
   STATIC_REQUIRE(1_km == 1'000_m);
   STATIC_REQUIRE(1_km + 1_m == 1'001_m);
   // STATIC_REQUIRE(10_km / 5_km == 2);
   STATIC_REQUIRE(10_km / 2 == 5_km);
   // STATIC_REQUIRE(100_mm / 5_cm == 2);

   // mass
   STATIC_REQUIRE(1_kg == 1'000_g);
   STATIC_REQUIRE(1_t == 1'000_kg);
   STATIC_REQUIRE(1_t == 1'000'000_g);

   // frequency
   STATIC_REQUIRE(2 / 1_s == 2_Hz);
   STATIC_REQUIRE(120 / 1_min == 2_Hz);
   STATIC_REQUIRE(1000 / 1_s == 1_kHz);
   STATIC_REQUIRE(1 / 1_ms == 1_kHz);
   // STATIC_REQUIRE(3.2_GHz == 3'200'000'000_Hz);
   // STATIC_REQUIRE(10_Hz * 1_min == 600);

   // force
   STATIC_REQUIRE(10_kg * 10_mps_sq == 100_N);
   STATIC_REQUIRE(1_kN == 1'000_N);
   STATIC_REQUIRE(1_MN == 1'000'000_N);

   // pressure
   STATIC_REQUIRE(10_N / 10_sq_m == 1_Pa);

   // energy
   STATIC_REQUIRE(10_N * 10_m == 100_J);
   // STATIC_REQUIRE(10Pa * 10cub_m == 100_J);

   // power
   STATIC_REQUIRE(10_J / 10_s == 1_W);

   // electric charge
   STATIC_REQUIRE(10_A * 10_s == 100_C);

   // voltage
   // STATIC_REQUIRE(10_W / 10_A == 1_V);
   // STATIC_REQUIRE(10_J / 10_C == 1_V);

   // capacitance
   // STATIC_REQUIRE(10_C / 10_V == 1_F);

   // velocity
   STATIC_REQUIRE(10_m / 5_s == 2_mps);
   STATIC_REQUIRE(1_km / 1_s == 1000_mps);
   STATIC_REQUIRE(10 / 5_s * 1_m == 2_mps);
   //STATIC_REQUIRE(2_kmph * 2_h == 4_km);
   //STATIC_REQUIRE(2_km / 2_kmph == 1_h);
   STATIC_REQUIRE(1_km / 1_s == 1000_mps);
   //STATIC_REQUIRE(1.0km / 1h == 1kmph);
   //STATIC_REQUIRE(1000.0m / 3600.0s == 1kmph);

   // acceleration
   STATIC_REQUIRE(10_mps / 10_s == 1_mps_sq);

   // area
   STATIC_REQUIRE(1_m * 1_m == 1_sq_m);
   // STATIC_REQUIRE(10_km * 10_km == 100_sq_km);
   //STATIC_REQUIRE(1sq_m == 10'000sq_cm);

   // volume
   STATIC_REQUIRE(1_m * 1_m * 1_m == 1_cub_m);
   STATIC_REQUIRE(10_sq_m * 10_m == 100_cub_m);
   //STATIC_REQUIRE(10_km * 10_km * 10_km == 1000_cub_km);
   //STATIC_REQUIRE(1_cub_m == 1'000'000_cub_cm);
}

TEST_CASE("constant quantities", "[sec21][units]")
{
   using namespace sec21::units::constants;
   using namespace sec21::units::literals;

   STATIC_REQUIRE(G == 9.80665_mps_sq);
}

TEST_CASE("nlohmann json reflection for units", "[sec21][serializer]")
{
   using namespace std::literals::string_literals;
   const auto temporary_test_file{"quantity.json"s};

   using nlohmann::json;
   using namespace sec21::units;

   using quantity_t = quantity<meter, double>;

   SECTION("write some data into a temporary json file")
   {
      auto q{5.12_m};
      json j = q;
      std::ofstream ofs{temporary_test_file};   
      ofs << std::setw(4) << j << std::endl;

      int z{7};
      REQUIRE(z == 7);
   }
   SECTION("read testdata from temporary json file")
   {
      json j;
      std::ifstream ifs{temporary_test_file};
      ifs >> j;
      auto q = j.get<quantity_t>();

      REQUIRE(q.value() == 5.12);
   }
}

#if 0
template<class Tuple, template<class> class Predicate>
class find_in_if
{
    template<
        class T  // actual type
      , template<class> class P  // predicate
      , std::size_t I  // index within a tuple
      >
    struct box
    {
        using                        type  = T;
        static constexpr std::size_t index = I;
        static constexpr bool        value = P<T>::value;  // disjunction uses this
    };
  
    template<class, class = void>
    struct find_in_if_impl;
    
    template<std::size_t... Is, class _>
    struct find_in_if_impl<std::index_sequence<Is...>, _>
    {
        using type =
            std::disjunction<
                box<std::tuple_element_t<Is, Tuple>, Predicate, Is>...
              , box<struct not_found, Predicate, std::tuple_size_v<Tuple>>  // fallback
            >;
    };
    
 public:
    using type = typename find_in_if_impl<std::make_index_sequence<std::tuple_size_v<Tuple>>>::type;
};

template<class Tuple, template<class> class Predicate>
using find_in_if_t = typename find_in_if<Tuple, Predicate>::type;

template<typename T>
auto add_generic(T first, std::string_view unit) -> std::tuple<bool, T>
{
    auto b1 = (unit == abbreviation<T>::value);
    std::cout << "check abbreviation: " << abbreviation<T>::value << " equal: " << std::boolalpha << b1 << std::endl;
    return std::tuple{ b1, first };
}


template<typename Tuple>
constexpr auto array_from_tuple(Tuple&& tuple)
{
    constexpr auto get_array = [](auto&& ... x){ return std::array{std::forward<decltype(x)>(x)... }; };
    return std::apply(get_array, std::forward<Tuple>(tuple));
}

#endif