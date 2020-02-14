#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "approx_equal.h"

#include <sec21/units.h>
#include <sec21/flat_matrix.h>
// #include <sec21/structural_analysis/axis.h>
// #include <sec21/structural_analysis/lookup.h>
#include <sec21/structural_analysis/space_truss.h>
#include <sec21/structural_analysis/system_result.h>
// test impl
#include <sec21/structural_analysis/impl/zuordnungsmatrix.h>
#include <sec21/structural_analysis/impl/geometry.h>
#include <sec21/structural_analysis/impl/stiffness_matrix.h>
#include <sec21/structural_analysis/impl/global_stiffness_matrix.h>
#include <sec21/structural_analysis/impl/steifigkeitsbeziehung_fachwerkstab_globalen_koordinaten.h>

// #include <boost/numeric/ublas/matrix_proxy.hpp>
// #include <boost/numeric/ublas/io.hpp>

#include <omp.h>

#include <vector>
#include <array>
// #include <map>
#include <utility>
#include <algorithm>

// is_singulaer(A) { return det(A) == 0; }

// linear algebra c++23
// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p1385r1.html

//! \todo possible to implement a string_type (percent) with a value range from 0...100%
constexpr auto kDivergence{0.02};

#include <string_view>
using namespace std::string_view_literals;

template <typename Iterator>
auto dump(Iterator first, Iterator last, std::string_view name)
{
   std::cout << name.data() << "\n(";
   std::copy(first, last, std::ostream_iterator<typename std::iterator_traits<Iterator>::value_type>(std::cout, ", "));
   std::cout << ")\n" << std::endl;
}

template <typename T>
auto dump(std::vector<T> const& seq, std::string_view name)
{
   return dump(std::begin(seq), std::end(seq), std::move(name));
}

//! \todo 2019-09-09: write a the same test, but reorder the nodes to check the lookup-tables
TEST_CASE("example system 1.0", "[sec21][structural_analysis][space_truss]")
{
   using namespace sec21::structural_analysis;
   using namespace sec21::units::literals;

   auto sys = space_truss{};

   using precision_t = decltype(sys)::precision_t;

   auto n1 = add_node(sys, {1, {0.0, 3.0}});
   auto n2 = add_node(sys, {2, {3.0, 3.0}, Support::Free, space_truss::force_t{{10.0_kN, -10.0_kN}}});
   auto n3 = add_node(sys, {3, {3.0, 0.0}, Support::Roller});
   auto n4 = add_node(sys, {4, {0.0, 0.0}, Support::Hinge});

   REQUIRE(static_cast<bool>(n1) == true);
   REQUIRE(static_cast<bool>(n2) == true);
   REQUIRE(static_cast<bool>(n3) == true);
   REQUIRE(static_cast<bool>(n4) == true);

   // constexpr auto E = 210.0_kPa;
   // constexpr auto A = unit::square_meter<double>(0.004);

   auto m1 = add_member(sys, 1, 2, {1, 0.004, 210'000'000});
   auto m2 = add_member(sys, 3, 2, {2, 0.004, 210'000'000});
   auto m3 = add_member(sys, 4, 3, {3, 0.004, 210'000'000});
   auto m4 = add_member(sys, 4, 1, {4, 0.004, 210'000'000});
   auto m5 = add_member(sys, 4, 2, {5, 0.004, 210'000'000});
   auto m6 = add_member(sys, 3, 1, {6, 0.004, 210'000'000});

   REQUIRE(static_cast<bool>(m1) == true);
   REQUIRE(static_cast<bool>(m2) == true);
   REQUIRE(static_cast<bool>(m3) == true);
   REQUIRE(static_cast<bool>(m4) == true);
   REQUIRE(static_cast<bool>(m5) == true);
   REQUIRE(static_cast<bool>(m6) == true);

   SECTION("test the geometry of the system")
   {
      REQUIRE(length(sys, m1.value()) == 3.0_m);
      REQUIRE(length(sys, m2.value()) == 3.0_m);
      REQUIRE(length(sys, m3.value()) == 3.0_m);
      REQUIRE(length(sys, m4.value()) == 3.0_m);
      REQUIRE(length(sys, m5.value()).value() == Approx(4.24264)); //_m);
      REQUIRE(length(sys, m6.value()).value() == Approx(4.24264)); //_m);

      namespace bmc = boost::math::constants;
      const auto fourth_pi{bmc::half_pi<precision_t>() * 0.5};

      REQUIRE(impl::angle_to_x_axis(sys, m1.value()) == Approx(0.0));
      REQUIRE(impl::angle_to_x_axis(sys, m2.value()) == Approx(bmc::half_pi<precision_t>()));
      REQUIRE(impl::angle_to_x_axis(sys, m3.value()) == Approx(0.0));
      REQUIRE(impl::angle_to_x_axis(sys, m4.value()) == Approx(bmc::half_pi<precision_t>()));
      REQUIRE(impl::angle_to_x_axis(sys, m5.value()) == Approx(fourth_pi));
      REQUIRE(impl::angle_to_x_axis(sys, m6.value()) == Approx(-fourth_pi));
   }
   SECTION("create lookup table") 
   {
      using namespace sec21::structural_analysis::impl;
      auto lookup = make_lookup(sys, Row{1});
      const auto expected = std::array{ Row{1}, Row{2}, Row{3}, Row{4}, Row{5}, Row{6}, Row{7}, Row{8} };

      REQUIRE(std::size(lookup) == std::size(expected));
      REQUIRE(std::equal(std::begin(lookup), std::end(lookup), std::begin(expected)));
   }
   SECTION("filter supported nodes from lookup table") 
   {
      using namespace sec21::structural_analysis::impl;
      auto lookup = make_lookup(sys, Row{0});
      const auto expected = std::array{ Row{0}, Row{1}, Row{2}, Row{3}, Row{4}, Row{5}, Row{6}, Row{7} };

      REQUIRE(std::size(lookup) == std::size(expected));
      REQUIRE(std::equal(std::begin(lookup), std::end(lookup), std::begin(expected)));

      auto result = filter(sys, lookup, is_node_in_dir_supported);
      const auto expected_festgehaltende_zeilen = std::vector{ Row{5}, Row{6}, Row{7} };

      REQUIRE(std::size(result) == std::size(expected_festgehaltende_zeilen));
      REQUIRE(std::equal(std::begin(result), std::end(result), std::begin(expected_festgehaltende_zeilen)));
   }
   SECTION("Steifigkeitsbeziehung vom Fachwerkstab 1 in globalen Koordinaten")
   {
      auto result = impl::steifigkeitsbeziehung_fachwerkstab_globalen_koordinaten(sys, m1.value());
      REQUIRE(result.size1() == 4);
      REQUIRE(result.size2() == 4);

      //! \clang-format off
      ///
      /// Stab 1
      /// ------
      /// Fx1            1  0 -1  0      u1 
      /// Fy1 = E*A/l *  0  0  0  0   *  v1
      /// Fx2           -1  0  1  0      u2
      /// Fy2            0  0  0  0      v2
      ///
      const std::array expected{
         1, 0,-1, 0,
         0, 0, 0, 0,
        -1, 0, 1, 0,
         0, 0, 0, 0
      };
      //! \clang-format on
      REQUIRE(approx_equal(sec21::flat_matrix(result), expected, kDivergence));
   }
   SECTION("Steifigkeitsbeziehung vom Fachwerkstab 2 in globalen Koordinaten")
   {
      auto result = impl::steifigkeitsbeziehung_fachwerkstab_globalen_koordinaten(sys, m2.value());
      REQUIRE(result.size1() == 4);
      REQUIRE(result.size2() == 4);

      //! \clang-format off
      ///
      /// Stab 2
      /// ------
      /// Fx3            0  0  0  0      u3 
      /// Fy3 = E*A/l *  0  1  0 -1   *  v3
      /// Fx2            0  0  0  0      u2
      /// Fy2            0 -1  0  1      v2
      ///
      const std::array expected{
         0, 0, 0, 0,
         0, 1, 0,-1,
         0, 0, 0, 0,
         0,-1, 0, 1
      };
      //! \clang-format on
      REQUIRE(approx_equal(sec21::flat_matrix(result), expected, kDivergence));
   }
   SECTION("Steifigkeitsbeziehung vom Fachwerkstab 3 in globalen Koordinaten")
   {
      auto result = impl::steifigkeitsbeziehung_fachwerkstab_globalen_koordinaten(sys, m3.value());
      REQUIRE(result.size1() == 4);
      REQUIRE(result.size2() == 4);

      //! \clang-format off
      ///
      /// Stab 3
      /// ------
      /// Fx4            1  0 -1  0      u4 
      /// Fy4 = E*A/l *  0  0  0  0   *  v4
      /// Fx3           -1  0  1  0      u3
      /// Fy3            0  0  0  0      v3
      ///
      const std::array expected{
         1, 0,-1, 0,
         0, 0, 0, 0,
        -1, 0, 1, 0,
         0, 0, 0, 0
      };
      //! \clang-format on
      REQUIRE(approx_equal(sec21::flat_matrix(result), expected, kDivergence));
   }
   SECTION("Steifigkeitsbeziehung vom Fachwerkstab 4 in globalen Koordinaten")
   {
      auto result = impl::steifigkeitsbeziehung_fachwerkstab_globalen_koordinaten(sys, m4.value());
      REQUIRE(result.size1() == 4);
      REQUIRE(result.size2() == 4);

      //! \clang-format off
      ///
      /// Stab 4
      /// ------
      /// Fx1            0  0  0  0      u1 
      /// Fy1 = E*A/l *  0  1  0  -1  *  v1
      /// Fx4            0  0  0  0      u4
      /// Fy4            0  -1  0  1     v4
      ///
      const std::array expected{
         0, 0, 0, 0,
         0, 1, 0,-1,
         0, 0, 0, 0,
         0,-1, 0, 1
      };
      //! \clang-format on
      REQUIRE(approx_equal(sec21::flat_matrix(result), expected, kDivergence));
   }         
   SECTION("Steifigkeitsbeziehung vom Fachwerkstab 5 in globalen Koordinaten")
   {
      auto result = impl::steifigkeitsbeziehung_fachwerkstab_globalen_koordinaten(sys, m5.value());
      REQUIRE(result.size1() == 4);
      REQUIRE(result.size2() == 4);
      //! \clang-format off
      const std::array expected{
          0.35355, 0.35355,-0.35355,-0.35355,
          0.35355, 0.35355,-0.35355,-0.35355,
         -0.35355,-0.35355, 0.35355, 0.35355,
         -0.35355,-0.35355, 0.35355, 0.35355
      };
      //! \clang-format on
      REQUIRE(approx_equal(sec21::flat_matrix(result), expected, kDivergence));
   }
   SECTION("Steifigkeitsbeziehung vom Fachwerkstab 6 in globalen Koordinaten")
   {
      auto result = impl::steifigkeitsbeziehung_fachwerkstab_globalen_koordinaten(sys, m6.value());
      REQUIRE(result.size1() == 4);
      REQUIRE(result.size2() == 4);
      //! \clang-format off
      const std::array expected{
          0.35355,-0.35355,-0.35355, 0.35355,
         -0.35355, 0.35355, 0.35355,-0.35355,
         -0.35355, 0.35355, 0.35355,-0.35355,
          0.35355,-0.35355,-0.35355, 0.35355
      };
      //! \clang-format on
      REQUIRE(approx_equal(sec21::flat_matrix(result), expected, kDivergence));
   }
   SECTION("Zuordnungsmatrix vom Fachwerkstab 1")
   {
      auto Z = impl::zuordnungsmatrix(sys, m1.value());
      
      REQUIRE(Z.size1() == 4);
      REQUIRE(Z.size2() == 8);
      //! \clang-format off
      const auto expected = std::array{
         1, 0, 0, 0, 0, 0, 0, 0,
         0, 1, 0, 0, 0, 0, 0, 0,
         0, 0, 1, 0, 0, 0, 0, 0,
         0, 0, 0, 1, 0, 0, 0, 0,
      };
      //! \clang-format on
      REQUIRE(approx_equal(sec21::flat_matrix(Z), expected, kDivergence));
   }
   SECTION("Zuordnungsmatrix vom Fachwerkstab 2")
   {
      auto Z = impl::zuordnungsmatrix(sys, m2.value());
      
      REQUIRE(Z.size1() == 4);
      REQUIRE(Z.size2() == 8);
      //! \clang-format off
      const auto expected = std::array{
         0, 0, 0, 0, 1, 0, 0, 0,
         0, 0, 0, 0, 0, 1, 0, 0,
         0, 0, 1, 0, 0, 0, 0, 0,
         0, 0, 0, 1, 0, 0, 0, 0,
      };
      //! \clang-format on
      REQUIRE(approx_equal(sec21::flat_matrix(Z), expected, kDivergence));
   }
   SECTION("Zuordnungsmatrix vom Fachwerkstab 3")
   {
      auto Z = impl::zuordnungsmatrix(sys, m3.value());
      
      REQUIRE(Z.size1() == 4);
      REQUIRE(Z.size2() == 8);
      //! \clang-format off
      const auto expected = std::array{
         0, 0, 0, 0, 0, 0, 1, 0,
         0, 0, 0, 0, 0, 0, 0, 1,
         0, 0, 0, 0, 1, 0, 0, 0,
         0, 0, 0, 0, 0, 1, 0, 0,
      };
      //! \clang-format on
      REQUIRE(approx_equal(sec21::flat_matrix(Z), expected, kDivergence));
   }
   SECTION("Zuordnungsmatrix vom Fachwerkstab 4")
   {
      auto Z = impl::zuordnungsmatrix(sys, m4.value());
      
      REQUIRE(Z.size1() == 4);
      REQUIRE(Z.size2() == 8);
      //! \clang-format off
      const auto expected = std::array{
         0, 0, 0, 0, 0, 0, 1, 0,
         0, 0, 0, 0, 0, 0, 0, 1,
         1, 0, 0, 0, 0, 0, 0, 0,
         0, 1, 0, 0, 0, 0, 0, 0,
      };
      //! \clang-format on
      REQUIRE(approx_equal(sec21::flat_matrix(Z), expected, kDivergence));
   }     
   SECTION("Zuordnungsmatrix vom Fachwerkstab 5")
   {
      auto Z = impl::zuordnungsmatrix(sys, m5.value());
      
      REQUIRE(Z.size1() == 4);
      REQUIRE(Z.size2() == 8);
      //! \clang-format off
      const auto expected = std::array{
         0, 0, 0, 0, 0, 0, 1, 0,
         0, 0, 0, 0, 0, 0, 0, 1,
         0, 0, 1, 0, 0, 0, 0, 0,
         0, 0, 0, 1, 0, 0, 0, 0,
      };
      //! \clang-format on
      REQUIRE(approx_equal(sec21::flat_matrix(Z), expected, kDivergence));
   }
   SECTION("Zuordnungsmatrix vom Fachwerkstab 6")
   {
      auto Z = impl::zuordnungsmatrix(sys, m6.value());
      
      REQUIRE(Z.size1() == 4);
      REQUIRE(Z.size2() == 8);
      //! \clang-format off
      const auto expected = std::array{
         0, 0, 0, 0, 1, 0, 0, 0,
         0, 0, 0, 0, 0, 1, 0, 0,
         1, 0, 0, 0, 0, 0, 0, 0,
         0, 1, 0, 0, 0, 0, 0, 0,
      };
      //! \clang-format on
      REQUIRE(approx_equal(sec21::flat_matrix(Z), expected, kDivergence));
   }   
   SECTION("Steifigkeitsmatrix vom Fachwerkstab 1")
   {
      auto K = impl::stiffness_matrix(sys, m1.value());
      REQUIRE(K.size1() == 8);
      REQUIRE(K.size2() == 8);
      //! \clang-format off
      const auto expected = std::array{
         1.0, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0,  0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
        -1.0, 0.0,  1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0,  0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0,  0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0,  0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0,  0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0,  0.0, 0.0, 0.0, 0.0, 0.0, 0.0
      };
      //! \clang-format on
      const auto flattend_K = sec21::flat_matrix(K);
      REQUIRE(std::size(flattend_K) == std::size(expected));
      REQUIRE(approx_equal(flattend_K, expected, kDivergence));
   }   
   SECTION("Steifigkeitsmatrix vom Fachwerkstab 2")
   {
      auto K = impl::stiffness_matrix(sys, m2.value());
      REQUIRE(K.size1() == 8);
      REQUIRE(K.size2() == 8);
      //! \clang-format off
      const auto expected = std::array{
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 1.0, 0.0,-1.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0,-1.0, 0.0, 1.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
      };
      //! \clang-format on
      const auto flattend_K = sec21::flat_matrix(K);
      REQUIRE(std::size(flattend_K) == std::size(expected));
      REQUIRE(approx_equal(flattend_K, expected, kDivergence));
   }
   SECTION("Steifigkeitsmatrix vom Fachwerkstab 3")
   {
      auto K = impl::stiffness_matrix(sys, m3.value());
      REQUIRE(K.size1() == 8);
      REQUIRE(K.size2() == 8);
      //! \clang-format off
      const auto expected = std::array{
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 1.0, 0.0,-1.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0,-1.0, 0.0, 1.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
      };
      //! \clang-format on
      const auto flattend_K = sec21::flat_matrix(K);
      REQUIRE(std::size(flattend_K) == std::size(expected));
      REQUIRE(approx_equal(flattend_K, expected, kDivergence));
   }
   SECTION("Steifigkeitsmatrix vom Fachwerkstab 4")
   {
      auto K = impl::stiffness_matrix(sys, m4.value());
      REQUIRE(K.size1() == 8);
      REQUIRE(K.size2() == 8);
      //! \clang-format off
      const auto expected = std::array{
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,-1.0,
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0,-1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0
      };
      //! \clang-format on
      const auto flattend_K = sec21::flat_matrix(K);
      REQUIRE(std::size(flattend_K) == std::size(expected));
      REQUIRE(approx_equal(flattend_K, expected, kDivergence));
   }      
   SECTION("Steifigkeitsmatrix vom Fachwerkstab 5")
   {
      auto K = impl::stiffness_matrix(sys, m5.value());
      REQUIRE(K.size1() == 8);
      REQUIRE(K.size2() == 8);
      //! \todo consider angle (anteile (EA) des stabes in x und y achse)
      // K *= (1 / 1.4142135623730950488);
      //! \clang-format off
      const auto expected = std::array{
         0.0, 0.0,  0.0,  0.0,  0.0, 0.0,  0.0,   0.0,
         0.0, 0.0,  0.0,  0.0,  0.0, 0.0,  0.0,   0.0,
         0.0, 0.0,  0.35, 0.35, 0.0, 0.0, -0.35, -0.35,
         0.0, 0.0,  0.35, 0.35, 0.0, 0.0, -0.35, -0.35,
         0.0, 0.0,  0.0,  0.0,  0.0, 0.0,  0.0,   0.0,
         0.0, 0.0,  0.0,  0.0,  0.0, 0.0,  0.0,   0.0,
         0.0, 0.0, -0.35,-0.35, 0.0, 0.0,  0.35,  0.35,
         0.0, 0.0, -0.35,-0.35, 0.0, 0.0,  0.35,  0.35
      };
      //! \clang-format on
      const auto flattend_K = sec21::flat_matrix(K);
      REQUIRE(std::size(flattend_K) == std::size(expected));
      REQUIRE(approx_equal(flattend_K, expected, kDivergence));
   }
   SECTION("Steifigkeitsmatrix vom Fachwerkstab 6")
   {
      auto K = impl::stiffness_matrix(sys, m6.value());
      REQUIRE(K.size1() == 8);
      REQUIRE(K.size2() == 8);

      //! \clang-format off
      const auto expected = std::array{
         0.35,-0.35, 0.0, 0.0,-0.35, 0.35, 0.0, 0.0,
        -0.35, 0.35, 0.0, 0.0, 0.35,-0.35, 0.0, 0.0,
         0.0,  0.0,  0.0, 0.0, 0.0,  0.0,  0.0, 0.0,
         0.0,  0.0,  0.0, 0.0, 0.0,  0.0,  0.0, 0.0,
        -0.35, 0.35, 0.0, 0.0, 0.35,-0.35, 0.0, 0.0,
         0.35,-0.35, 0.0, 0.0,-0.35, 0.35, 0.0, 0.0,
         0.0,  0.0,  0.0, 0.0, 0.0,  0.0,  0.0, 0.0,
         0.0,  0.0,  0.0, 0.0, 0.0,  0.0,  0.0, 0.0,
      };
      //! \clang-format on
      const auto flattend_K = sec21::flat_matrix(K);
      REQUIRE(std::size(flattend_K) == std::size(expected));
      REQUIRE(approx_equal(flattend_K, expected, kDivergence));
   }     
   SECTION("Systemsteifigkeitsmatrix (K_f) ohne Berücksichtigung der Auflagerbedingungen")
   {
      auto K = impl::global_stiffness_matrix(sys);
      REQUIRE(K.size1() == 8);
      REQUIRE(K.size2() == 8);

      //! \clang-format off
      const auto expected = std::array{
          1.35, -0.35,-1.0,  0.0, -0.35, 0.35, 0.0,   0.0,
         -0.35,  1.35, 0.0,  0.0,  0.35,-0.35, 0.0,  -1.0,
         -1.0,   0.0,  1.35, 0.35, 0.0,  0.0, -0.35, -0.35,
          0.0,   0.0,  0.35, 1.35, 0.0, -1.0, -0.35, -0.35,
         -0.35,  0.35, 0.0,  0.0,  1.35,-0.35,-1.0,   0.0,
          0.35, -0.35, 0.0, -1.0, -0.35, 1.35, 0.0,   0.0,
          0.0,   0.0, -0.35,-0.35,-1.0,  0.0,  1.35,  0.35,
          0.0,  -1.0, -0.35,-0.35, 0.0,  0.0,  0.35,  1.35
      };
      //! \clang-format on
      const auto flattend_K = sec21::flat_matrix(K);
      REQUIRE(std::size(flattend_K) == std::size(expected));
      REQUIRE(approx_equal(flattend_K, expected, kDivergence));
   }
   SECTION("filter supports from global stiffness matrix")
   {
      const auto lookup = impl::make_lookup(sys, impl::Row{0});
      const auto festgehaltende_zeilen = impl::filter(sys, lookup, impl::is_node_in_dir_supported);
      const auto festgehaltende_spalten = impl::row_to_col(std::begin(festgehaltende_zeilen), std::end(festgehaltende_zeilen));

      auto K = impl::global_stiffness_matrix(sys);
      auto K_without_supports = impl::remove_from_matrix(K, festgehaltende_zeilen, festgehaltende_spalten);
      
      REQUIRE(K_without_supports.size1() == 5);
      REQUIRE(K_without_supports.size2() == 5);
      // clang-format off
      //              |  1.35, -0.35, -1.0,  0.0, -0.35, |   | u1 |   | Fx1 |
      //              | -0.35,  1.35,  0.0,  0.0,  0.35, |   | v1 |   | Fy1 |
      // 2.8 * 10^3 * | -1.0,   0.0,   1.35  0.35, 0.0,  | * | u2 | = | Fx2 |
      //              |  0.0,   0.0,   0.35, 1.35, 0.0,  |   | v2 |   | Fy2 |
      //              | -0.35,  0.35,  0.0,  0.0,  1.35  |   | u3 |   | Fx3 |
      const auto expected = std::array{
          1.35, -0.35, -1.0,  0.0, -0.35,
         -0.35,  1.35,  0.0,  0.0,  0.35,
         -1.0,   0.0,   1.35, 0.35, 0.0,
          0.0,   0.0,   0.35, 1.35, 0.0,
         -0.35,  0.35,  0.0,  0.0,  1.35
      };
      // clang-format on
      const auto flattend_K = sec21::flat_matrix(K_without_supports);
      REQUIRE(std::size(flattend_K) == std::size(expected));      
      REQUIRE(approx_equal(flattend_K, expected, kDivergence));
   }
   SECTION("Lastvektor (F)")
   {
      auto F = impl::lastvektor_F(sys);
      const auto expected = std::array{ 0.0, 0.0, 10.0, -10.0, 0.0, 0.0, 0.0, 0.0 };
      REQUIRE(approx_equal(F, expected, kDivergence));
   }
   SECTION("K strich")
   {
      const auto n = std::distance(std::begin(sys.nodes), std::end(sys.nodes));
      const auto lookup = impl::make_lookup(sys, impl::Row{0});
      const auto festgehaltende_zeilen = impl::filter(sys, lookup, impl::is_node_in_dir_supported);
      const auto festgehaltende_spalten = impl::row_to_col(std::begin(festgehaltende_zeilen), std::end(festgehaltende_zeilen));
      //! \todo const -> 
      //! \todo more like filter() with is_node_in_dir_supported() == false
      auto verschiebungs_zeilen = std::vector<impl::Row>((n * decltype(sys)::dimension_v) - std::size(festgehaltende_spalten));
      std::iota(std::begin(verschiebungs_zeilen), std::end(verschiebungs_zeilen), impl::Row{0});

      auto K = impl::global_stiffness_matrix(sys);
      auto K_strich = impl::remove_from_matrix(K, verschiebungs_zeilen, festgehaltende_spalten);

      REQUIRE(K_strich.size1() == 3);
      REQUIRE(K_strich.size2() == 5);
      // clang-format off
      //                                                       | u1 |     
      //              |  0.35, -0.35,  0.0,  -1.0,  -0.35, |   | v1 |   | Fy3 | 
      // 2.8 * 10^3 * |  0.0,   0.0,  -0.35, -0.35, -1.0,  | * | u2 | = | Fx4 |  
      //              |  0.0,  -1.0,  -0.35  -0.35,  0.0   |   | v2 |   | Fy4 |
      //                                                       | u3 |
      const auto expected = std::array{
          0.35, -0.35, 0.0,  -1.0,  -0.35,
          0.0,   0.0, -0.35, -0.35, -1.0,
          0.0,  -1.0, -0.35, -0.35,  0.0
      };
      // clang-format on
      REQUIRE(approx_equal(sec21::flat_matrix(K_strich), expected, kDivergence));
   }
   SECTION("solve function")
   {
      auto [success, result] = solve(sys);
      REQUIRE(success == true);

      const auto expected_auflagerreaktionen = std::array{ 0.0, 0.0, 0.0, 0.0, 0.0, 20.0, -10.0, -10.0 };
      REQUIRE(result.auflagerreaktionen.size() == std::size(expected_auflagerreaktionen));
      REQUIRE(approx_equal(result.auflagerreaktionen, expected_auflagerreaktionen, kDivergence));

      const auto expected_verschiebungen = std::array{ 0.86, 0.18, 1.04, -0.54, 0.18, 0.0, 0.0, 0.0 };
      REQUIRE(std::size(result.verschiebungen) == std::size(expected_verschiebungen));
      REQUIRE(approx_equal(result.verschiebungen, expected_verschiebungen, kDivergence));

      const auto expected_N = std::array{ 5.0, -15.0, 5.0, 5.0, 7.07, -7.07 };
      REQUIRE(std::size(result.normalkraefte) == std::size(expected_N));
      REQUIRE(approx_equal(result.normalkraefte, expected_N, kDivergence));
   }

#ifdef CATCH_CONFIG_ENABLE_BENCHMARKING
   // BENCHMARK("Benchmark: Systemsteifigkeitsmatrix (K) als boost::numeric::ublas::matrix")
   // {
   //    auto K = global_stiffness_matrix(sys);
   //    return K;
   // };
   // BENCHMARK("Benchmark: Systemsteifigkeitsmatrix (K) als std::vector")
   // {
   //    auto K = v2::global_stiffness_matrix(sys);
   //    return K;
   // };
#endif
}

#ifdef __cpp_concepts

namespace ns
{
template <typename T>
auto do_something(T) requires sec21::structural_analysis::SpaceTruss<T> { return true; }
} // namespace ns

TEST_CASE("test concepts", "[concepts]")
{
   auto result = ns::do_something(sec21::structural_analysis::space_truss{});
   REQUIRE(result == true);
}
#endif

TEST_CASE("example 1.1 with different node id's (N1 switched with N3 from example 1)", "[sec21][structural_analysis][space_truss]")
{
   using namespace sec21::structural_analysis;
   using namespace sec21::units::literals;

   auto sys = space_truss{};
   using precision_t = decltype(sys)::precision_t;

   auto n1 = add_node(sys, {1, {3.0, 0.0}, Support::Roller});
   auto n2 = add_node(sys, {2, {3.0, 3.0}, Support::Free, space_truss::force_t{{10.0_kN, -10.0_kN}}});
   auto n3 = add_node(sys, {3, {0.0, 3.0}});
   auto n4 = add_node(sys, {4, {0.0, 0.0}, Support::Hinge});

   REQUIRE(static_cast<bool>(n1) == true);
   REQUIRE(static_cast<bool>(n2) == true);
   REQUIRE(static_cast<bool>(n3) == true);
   REQUIRE(static_cast<bool>(n4) == true);

   // constexpr auto E = 210.0_kPa;
   // constexpr auto A = unit::square_meter<double>(0.004);

   auto m1 = add_member(sys, 3, 2, {1, 0.004, 210'000'000});
   auto m2 = add_member(sys, 1, 2, {2, 0.004, 210'000'000});
   auto m3 = add_member(sys, 4, 1, {3, 0.004, 210'000'000});
   auto m4 = add_member(sys, 4, 3, {4, 0.004, 210'000'000});
   auto m5 = add_member(sys, 4, 2, {5, 0.004, 210'000'000});
   auto m6 = add_member(sys, 1, 3, {6, 0.004, 210'000'000});

   REQUIRE(static_cast<bool>(m1) == true);
   REQUIRE(static_cast<bool>(m2) == true);
   REQUIRE(static_cast<bool>(m3) == true);
   REQUIRE(static_cast<bool>(m4) == true);
   REQUIRE(static_cast<bool>(m5) == true);
   REQUIRE(static_cast<bool>(m6) == true);

   SECTION("test the geometry of the system")
   {
      REQUIRE(length(sys, m1.value()) == 3.0_m);
      REQUIRE(length(sys, m2.value()) == 3.0_m);
      REQUIRE(length(sys, m3.value()) == 3.0_m);
      REQUIRE(length(sys, m4.value()) == 3.0_m);
      REQUIRE(length(sys, m5.value()).value() == Approx(4.24264)); //_m);
      REQUIRE(length(sys, m6.value()).value() == Approx(4.24264)); //_m);

      namespace bmc = boost::math::constants;
      const auto fourth_pi{bmc::half_pi<precision_t>() * 0.5};

      REQUIRE(impl::angle_to_x_axis(sys, m1.value()) == Approx(0.0));
      REQUIRE(impl::angle_to_x_axis(sys, m2.value()) == Approx(bmc::half_pi<precision_t>()));
      REQUIRE(impl::angle_to_x_axis(sys, m3.value()) == Approx(0.0));
      REQUIRE(impl::angle_to_x_axis(sys, m4.value()) == Approx(bmc::half_pi<precision_t>()));
      REQUIRE(impl::angle_to_x_axis(sys, m5.value()) == Approx(fourth_pi));
      REQUIRE(impl::angle_to_x_axis(sys, m6.value()) == Approx(-fourth_pi));
   }
   SECTION("filter supported nodes from lookup table") 
   {
      using namespace sec21::structural_analysis::impl;
      auto lookup = impl::make_lookup(sys, Row{0});
      const auto expected = std::array{ Row{0}, Row{1}, Row{2}, Row{3}, Row{4}, Row{5}, Row{6}, Row{7} };

      REQUIRE(std::size(lookup) == std::size(expected));
      REQUIRE(std::equal(std::begin(lookup), std::end(lookup), std::begin(expected)));

      auto result = impl::filter(sys, lookup, is_node_in_dir_supported);
      const auto expected_festgehaltende_zeilen = std::vector{ Row{1}, Row{6}, Row{7} };

      REQUIRE(std::size(result) == std::size(expected_festgehaltende_zeilen));
      REQUIRE(std::equal(std::begin(result), std::end(result), std::begin(expected_festgehaltende_zeilen)));
   }   
   SECTION("Zuordnungsmatrix vom Fachwerkstab 1")
   {
      auto Z = impl::zuordnungsmatrix(sys, m1.value());
      
      REQUIRE(Z.size1() == 4);
      REQUIRE(Z.size2() == 8);
      //! \clang-format off
      const auto expected = std::array{
         0, 0, 0, 0, 1, 0, 0, 0,
         0, 0, 0, 0, 0, 1, 0, 0,
         0, 0, 1, 0, 0, 0, 0, 0,
         0, 0, 0, 1, 0, 0, 0, 0,
      };
      //! \clang-format on
      REQUIRE(approx_equal(sec21::flat_matrix(Z), expected, kDivergence));
   }
   SECTION("Zuordnungsmatrix vom Fachwerkstab 2")
   {
      auto Z = impl::zuordnungsmatrix(sys, m2.value());
      
      REQUIRE(Z.size1() == 4);
      REQUIRE(Z.size2() == 8);
      //! \clang-format off
      const auto expected = std::array{
         1, 0, 0, 0, 0, 0, 0, 0,
         0, 1, 0, 0, 0, 0, 0, 0,
         0, 0, 1, 0, 0, 0, 0, 0,
         0, 0, 0, 1, 0, 0, 0, 0,
      };
      //! \clang-format on
      REQUIRE(approx_equal(sec21::flat_matrix(Z), expected, kDivergence));
   }
   SECTION("Zuordnungsmatrix vom Fachwerkstab 3")
   {
      auto Z = impl::zuordnungsmatrix(sys, m3.value());
      
      REQUIRE(Z.size1() == 4);
      REQUIRE(Z.size2() == 8);
      //! \clang-format off
      const auto expected = std::array{
         0, 0, 0, 0, 0, 0, 1, 0,
         0, 0, 0, 0, 0, 0, 0, 1,
         1, 0, 0, 0, 0, 0, 0, 0,
         0, 1, 0, 0, 0, 0, 0, 0,
      };
      //! \clang-format on
      REQUIRE(approx_equal(sec21::flat_matrix(Z), expected, kDivergence));
   }
   SECTION("Zuordnungsmatrix vom Fachwerkstab 4")
   {
      auto Z = impl::zuordnungsmatrix(sys, m4.value());
      
      REQUIRE(Z.size1() == 4);
      REQUIRE(Z.size2() == 8);
      //! \clang-format off
      const auto expected = std::array{
         0, 0, 0, 0, 0, 0, 1, 0,
         0, 0, 0, 0, 0, 0, 0, 1,
         0, 0, 0, 0, 1, 0, 0, 0,
         0, 0, 0, 0, 0, 1, 0, 0,
      };
      //! \clang-format on
      REQUIRE(approx_equal(sec21::flat_matrix(Z), expected, kDivergence));
   }     
   SECTION("Zuordnungsmatrix vom Fachwerkstab 5")
   {
      auto Z = impl::zuordnungsmatrix(sys, m5.value());
      
      REQUIRE(Z.size1() == 4);
      REQUIRE(Z.size2() == 8);
      //! \clang-format off
      const auto expected = std::array{
         0, 0, 0, 0, 0, 0, 1, 0,
         0, 0, 0, 0, 0, 0, 0, 1,
         0, 0, 1, 0, 0, 0, 0, 0,
         0, 0, 0, 1, 0, 0, 0, 0,
      };
      //! \clang-format on
      REQUIRE(approx_equal(sec21::flat_matrix(Z), expected, kDivergence));
   }
   SECTION("Zuordnungsmatrix vom Fachwerkstab 6")
   {
      auto Z = impl::zuordnungsmatrix(sys, m6.value());
      
      REQUIRE(Z.size1() == 4);
      REQUIRE(Z.size2() == 8);
      //! \clang-format off
      const auto expected = std::array{
         1, 0, 0, 0, 0, 0, 0, 0,
         0, 1, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 1, 0, 0, 0,
         0, 0, 0, 0, 0, 1, 0, 0,
      };
      //! \clang-format on
      REQUIRE(approx_equal(sec21::flat_matrix(Z), expected, kDivergence));
   }   
   SECTION("solve")
   {
      auto [success, result] = solve(sys);
      REQUIRE(success == true);

      // clang-format off
      const auto expected_auflagerreaktionen = std::array{ 
         0.0,  20.0, 
         0.0,   0.0, 
         0.0,   0.0, 
       -10.0, -10.0 };
      // clang-format on
      REQUIRE(result.auflagerreaktionen.size() == std::size(expected_auflagerreaktionen));
      REQUIRE(approx_equal(result.auflagerreaktionen, expected_auflagerreaktionen, kDivergence));

      // clang-format off
      const auto expected_verschiebungen = std::array{ 
         0.18,  0.0, 
         1.04, -0.54, 
         0.86,  0.18, 
         0.0,   0.0 };
      // clang-format on
      REQUIRE(std::size(result.verschiebungen) == std::size(expected_verschiebungen));
      REQUIRE(approx_equal(result.verschiebungen, expected_verschiebungen, kDivergence));

      const auto expected_N = std::array{ 5.0, -15.0, 5.0, 5.0, 7.07, -7.07 };
      REQUIRE(std::size(result.normalkraefte) == std::size(expected_N));
      REQUIRE(approx_equal(result.normalkraefte, expected_N, kDivergence));
   }
}

TEST_CASE("example system 1.2 with half load from example 1.0", "[sec21][structural_analysis][space_truss]")
{
   using namespace sec21::structural_analysis;
   using namespace sec21::units::literals;

   auto sys = space_truss{};

   using precision_t = decltype(sys)::precision_t;

   auto n1 = add_node(sys, {1, {0.0, 3.0}});
   auto n2 = add_node(sys, {2, {3.0, 3.0}, Support::Free, space_truss::force_t{{5.0_kN, -5.0_kN}}});
   auto n3 = add_node(sys, {3, {3.0, 0.0}, Support::Roller});
   auto n4 = add_node(sys, {4, {0.0, 0.0}, Support::Hinge});

   REQUIRE(static_cast<bool>(n1) == true);
   REQUIRE(static_cast<bool>(n2) == true);
   REQUIRE(static_cast<bool>(n3) == true);
   REQUIRE(static_cast<bool>(n4) == true);

   // constexpr auto E = 210.0_kPa;
   // constexpr auto A = unit::square_meter<double>(0.004);

   auto m1 = add_member(sys, 1, 2, {1, 0.004, 210'000'000});
   auto m2 = add_member(sys, 3, 2, {2, 0.004, 210'000'000});
   auto m3 = add_member(sys, 4, 3, {3, 0.004, 210'000'000});
   auto m4 = add_member(sys, 4, 1, {4, 0.004, 210'000'000});
   auto m5 = add_member(sys, 4, 2, {5, 0.004, 210'000'000});
   auto m6 = add_member(sys, 3, 1, {6, 0.004, 210'000'000});

   REQUIRE(static_cast<bool>(m1) == true);
   REQUIRE(static_cast<bool>(m2) == true);
   REQUIRE(static_cast<bool>(m3) == true);
   REQUIRE(static_cast<bool>(m4) == true);
   REQUIRE(static_cast<bool>(m5) == true);
   REQUIRE(static_cast<bool>(m6) == true);

   SECTION("test the geometry of the system")
   {
      REQUIRE(length(sys, m1.value()) == 3.0_m);
      REQUIRE(length(sys, m2.value()) == 3.0_m);
      REQUIRE(length(sys, m3.value()) == 3.0_m);
      REQUIRE(length(sys, m4.value()) == 3.0_m);
      REQUIRE(length(sys, m5.value()).value() == Approx(4.24264)); //_m);
      REQUIRE(length(sys, m6.value()).value() == Approx(4.24264)); //_m);

      namespace bmc = boost::math::constants;
      const auto fourth_pi{bmc::half_pi<precision_t>() * 0.5};

      REQUIRE(impl::angle_to_x_axis(sys, m1.value()) == Approx(0.0));
      REQUIRE(impl::angle_to_x_axis(sys, m2.value()) == Approx(bmc::half_pi<precision_t>()));
      REQUIRE(impl::angle_to_x_axis(sys, m3.value()) == Approx(0.0));
      REQUIRE(impl::angle_to_x_axis(sys, m4.value()) == Approx(bmc::half_pi<precision_t>()));
      REQUIRE(impl::angle_to_x_axis(sys, m5.value()) == Approx(fourth_pi));
      REQUIRE(impl::angle_to_x_axis(sys, m6.value()) == Approx(-fourth_pi));
   }
   SECTION("solve function")
   {
      auto [success, result] = solve(sys);
      REQUIRE(success == true);

      const auto expected_auflagerreaktionen = std::array{ 0.0, 0.0, 0.0, 0.0, 0.0, 10.0, -5.0, -5.0 };
      REQUIRE(result.auflagerreaktionen.size() == std::size(expected_auflagerreaktionen));
      REQUIRE(approx_equal(result.auflagerreaktionen, expected_auflagerreaktionen, kDivergence));

      const auto expected_verschiebungen = std::array{ 0.43, 0.09, 0.52, -0.27, 0.09, 0.0, 0.0, 0.0 };
      REQUIRE(std::size(result.verschiebungen) == std::size(expected_verschiebungen));
      REQUIRE(approx_equal(result.verschiebungen, expected_verschiebungen, kDivergence));

      const auto expected_N = std::array{ 2.5, -7.5, 2.5, 2.5, 3.535, -3.535 };
      REQUIRE(std::size(result.normalkraefte) == std::size(expected_N));
      REQUIRE(approx_equal(result.normalkraefte, expected_N, kDivergence));
   }
}
