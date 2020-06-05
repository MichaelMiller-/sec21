#include <catch.hpp>
#include "approx_equal.h"

#include <sec21/access.h>
#include <sec21/units.h>
#include <sec21/flat_matrix.h>
#include <sec21/file_loader.h>
#include <sec21/structural_analysis/space_truss.h>
#include <sec21/structural_analysis/loadcase.h>
#include <sec21/structural_analysis/system_result.h>
#include <sec21/structural_analysis/solve.h>

#include <vector>
#include <array>
#include <valarray>
#include <utility>
#include <algorithm>

//! \todo possible to implement a strong_type (percent) with a value range from 0...100%
constexpr auto kDivergence{0.02};


TEST_CASE("example system 1.0", "[sec21][structural_analysis][space_truss]")
{
   using namespace sec21::structural_analysis;
   using namespace sec21::units::literals;

   auto sys = space_truss{};

   static_assert(is_space_truss<decltype(sys)>::value);
   static_assert(is_space_truss_2D<decltype(sys)>::value);

   using precision_t = decltype(sys)::precision_t;
   using node_t = decltype(sys)::node_t;
   using support_t = node_t::global_support_t;
   using loadcase_t = loadcase<decltype(sys)>;

   auto n1 = add_node(sys, {1, {0.0, 3.0}});
   auto n2 = add_node(sys, {2, {3.0, 3.0}});
   auto n3 = add_node(sys, {3, {3.0, 0.0}, support_t{ false, true }});
   auto n4 = add_node(sys, {4, {0.0, 0.0}, support_t{ true, true }});

   REQUIRE(static_cast<bool>(n1) == true);
   REQUIRE(static_cast<bool>(n2) == true);
   REQUIRE(static_cast<bool>(n3) == true);
   REQUIRE(static_cast<bool>(n4) == true);

   //! \todo use units
   // constexpr auto E = 210.0_kPa;
   // constexpr auto A = unit::square_meter<double>(0.004);
   auto m1 = add_member(sys, 1, 2, {1, 0.004, 21'000'000});
   auto m2 = add_member(sys, 3, 2, {2, 0.004, 21'000'000});
   auto m3 = add_member(sys, 4, 3, {3, 0.004, 21'000'000});
   auto m4 = add_member(sys, 4, 1, {4, 0.004, 21'000'000});
   auto m5 = add_member(sys, 4, 2, {5, 0.004, 21'000'000});
   auto m6 = add_member(sys, 3, 1, {6, 0.004, 21'000'000});

   const auto EA_l1 = 28'000;
   const auto EA_l2 = 19'798.98987;

   REQUIRE(static_cast<bool>(m1) == true);
   REQUIRE(static_cast<bool>(m2) == true);
   REQUIRE(static_cast<bool>(m3) == true);
   REQUIRE(static_cast<bool>(m4) == true);
   REQUIRE(static_cast<bool>(m5) == true);
   REQUIRE(static_cast<bool>(m6) == true);

   {
      std::ofstream ofs{"output_example_1.json"};
      nlohmann::json j = sys;
      ofs << std::setw(4) << j;
   }

   SECTION("test the geometry of the system")
   {
      REQUIRE(impl::length(sys, m1.value()) == 3.0_m);
      REQUIRE(impl::length(sys, m2.value()) == 3.0_m);
      REQUIRE(impl::length(sys, m3.value()) == 3.0_m);
      REQUIRE(impl::length(sys, m4.value()) == 3.0_m);
      REQUIRE(impl::length(sys, m5.value()).value() == Approx(4.24264)); //_m);
      REQUIRE(impl::length(sys, m6.value()).value() == Approx(4.24264)); //_m);

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

      std::vector<bool> mask;
      support_mask(std::begin(sys.nodes), std::end(sys.nodes), std::back_inserter(mask));

      decltype(lookup) supported_nodes;
      decltype(lookup) not_supported_nodes;

      partition_lookup(
         std::begin(lookup), 
         std::end(lookup),
         std::begin(mask), 
         std::end(mask),
         std::back_inserter(supported_nodes),
         std::back_inserter(not_supported_nodes));

      const auto expected_rows = std::vector{ Row{5}, Row{6}, Row{7} };

      REQUIRE(std::size(supported_nodes) == std::size(expected_rows));
      REQUIRE(std::equal(std::begin(supported_nodes), std::end(supported_nodes), std::begin(expected_rows)));
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
      auto expected = std::valarray{
         1, 0,-1, 0,
         0, 0, 0, 0,
        -1, 0, 1, 0,
         0, 0, 0, 0
      };
      expected *= EA_l1;
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
      auto expected = std::valarray{
         0, 0, 0, 0,
         0, 1, 0,-1,
         0, 0, 0, 0,
         0,-1, 0, 1
      };
      expected *= EA_l1;
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
      auto expected = std::valarray{
         1, 0,-1, 0,
         0, 0, 0, 0,
        -1, 0, 1, 0,
         0, 0, 0, 0
      };
      expected *= EA_l1;
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
      auto expected = std::valarray{
         0, 0, 0, 0,
         0, 1, 0,-1,
         0, 0, 0, 0,
         0,-1, 0, 1
      };
      expected *= EA_l1;
      //! \clang-format on
      REQUIRE(approx_equal(sec21::flat_matrix(result), expected, kDivergence));
   }
   SECTION("Steifigkeitsbeziehung vom Fachwerkstab 5 in globalen Koordinaten")
   {
      auto result = impl::steifigkeitsbeziehung_fachwerkstab_globalen_koordinaten(sys, m5.value());
      REQUIRE(result.size1() == 4);
      REQUIRE(result.size2() == 4);
      //! \clang-format off
      auto expected = std::valarray{
          0.5, 0.5,-0.5,-0.5,
          0.5, 0.5,-0.5,-0.5,
         -0.5,-0.5, 0.5, 0.5,
         -0.5,-0.5, 0.5, 0.5
      };
      expected *= EA_l2; // E*A / l
      //! \clang-format on
      REQUIRE(approx_equal(sec21::flat_matrix(result), expected, kDivergence));
   }
   SECTION("Steifigkeitsbeziehung vom Fachwerkstab 6 in globalen Koordinaten")
   {
      auto result = impl::steifigkeitsbeziehung_fachwerkstab_globalen_koordinaten(sys, m6.value());
      REQUIRE(result.size1() == 4);
      REQUIRE(result.size2() == 4);
      //! \clang-format off
      auto expected = std::valarray{
          0.5,-0.5,-0.5, 0.5,
         -0.5, 0.5, 0.5,-0.5,
         -0.5, 0.5, 0.5,-0.5,
          0.5,-0.5,-0.5, 0.5
      };
      expected *= EA_l2; // E*A / l
      //! \clang-format on
      REQUIRE(approx_equal(sec21::flat_matrix(result), expected, kDivergence));
   }
   SECTION("coincidence matrix from member 1")
   {
      auto Z = impl::coincidence_matrix(sys, m1.value());
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
   SECTION("coincidence matrix from member 2")
   {
      auto Z = impl::coincidence_matrix(sys, m2.value());
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
   SECTION("coincidence matrix from member 3")
   {
      auto Z = impl::coincidence_matrix(sys, m3.value());
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
   SECTION("coincidence matrix from member 4")
   {
      auto Z = impl::coincidence_matrix(sys, m4.value());
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
   SECTION("coincidence matrix from member 5")
   {
      auto Z = impl::coincidence_matrix(sys, m5.value());
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
   SECTION("coincidence matrix from member 6")
   {
      auto Z = impl::coincidence_matrix(sys, m6.value());
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
   SECTION("stiffness matrix from member 1")
   {
      auto K = impl::stiffness_matrix(sys, m1.value());
      REQUIRE(K.size1() == 8);
      REQUIRE(K.size2() == 8);
      //! \clang-format off
      auto expected = std::valarray{
         1.0, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0,  0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
        -1.0, 0.0,  1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0,  0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0,  0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0,  0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0,  0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0,  0.0, 0.0, 0.0, 0.0, 0.0, 0.0
      };
      expected *= EA_l1;
      //! \clang-format on
      const auto flattend_K = sec21::flat_matrix(K);
      REQUIRE(std::size(flattend_K) == std::size(expected));
      REQUIRE(approx_equal(flattend_K, expected, kDivergence));
   }   
   SECTION("stiffness matrix from member 2")
   {
      auto K = impl::stiffness_matrix(sys, m2.value());
      REQUIRE(K.size1() == 8);
      REQUIRE(K.size2() == 8);
      //! \clang-format off
      auto expected = std::valarray{
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 1.0, 0.0,-1.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0,-1.0, 0.0, 1.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
      };
      expected *= EA_l1;
      //! \clang-format on
      const auto flattend_K = sec21::flat_matrix(K);
      REQUIRE(std::size(flattend_K) == std::size(expected));
      REQUIRE(approx_equal(flattend_K, expected, kDivergence));
   }
   SECTION("stiffness matrix from member 3")
   {
      auto K = impl::stiffness_matrix(sys, m3.value());
      REQUIRE(K.size1() == 8);
      REQUIRE(K.size2() == 8);
      //! \clang-format off
      auto expected = std::valarray{
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 1.0, 0.0,-1.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0,-1.0, 0.0, 1.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
      };
      expected *= EA_l1;
      //! \clang-format on
      const auto flattend_K = sec21::flat_matrix(K);
      REQUIRE(std::size(flattend_K) == std::size(expected));
      REQUIRE(approx_equal(flattend_K, expected, kDivergence));
   }
   SECTION("stiffness matrix from member 4")
   {
      auto K = impl::stiffness_matrix(sys, m4.value());
      REQUIRE(K.size1() == 8);
      REQUIRE(K.size2() == 8);
      //! \clang-format off
      auto expected = std::valarray{
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,-1.0,
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0,-1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0
      };
      expected *= EA_l1;
      //! \clang-format on
      const auto flattend_K = sec21::flat_matrix(K);
      REQUIRE(std::size(flattend_K) == std::size(expected));
      REQUIRE(approx_equal(flattend_K, expected, kDivergence));
   }
   SECTION("stiffness matrix from member 5")
   {
      auto K = impl::stiffness_matrix(sys, m5.value());
      REQUIRE(K.size1() == 8);
      REQUIRE(K.size2() == 8);
      //! \clang-format off
      auto expected = std::valarray{
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.5, 0.5, 0.0, 0.0,-0.5,-0.5,
         0.0, 0.0, 0.5, 0.5, 0.0, 0.0,-0.5,-0.5,
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0,-0.5,-0.5, 0.0, 0.0, 0.5, 0.5,
         0.0, 0.0,-0.5,-0.5, 0.0, 0.0, 0.5, 0.5
      };
      expected *= EA_l2;
      //! \clang-format on
      const auto flattend_K = sec21::flat_matrix(K);
      REQUIRE(std::size(flattend_K) == std::size(expected));
      REQUIRE(approx_equal(flattend_K, expected, kDivergence));
   }
   SECTION("stiffness matrix from member 6")
   {
      auto K = impl::stiffness_matrix(sys, m6.value());
      REQUIRE(K.size1() == 8);
      REQUIRE(K.size2() == 8);
      //! \clang-format off
      auto expected = std::valarray{
         0.5,-0.5, 0.0, 0.0,-0.5, 0.5, 0.0, 0.0,
        -0.5, 0.5, 0.0, 0.0, 0.5,-0.5, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
        -0.5, 0.5, 0.0, 0.0, 0.5,-0.5, 0.0, 0.0,
         0.5,-0.5, 0.0, 0.0,-0.5, 0.5, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      };
      expected *= EA_l2;
      //! \clang-format on
      const auto flattend_K = sec21::flat_matrix(K);
      REQUIRE(std::size(flattend_K) == std::size(expected));
      REQUIRE(approx_equal(flattend_K, expected, kDivergence));
   }
   SECTION("global stiffness matrix")
   {
      auto K = impl::global_stiffness_matrix(sys);
      REQUIRE(K.size1() == 8);
      REQUIRE(K.size2() == 8);
      //! \clang-format off
      auto expected = std::valarray{
          1.35, -0.35,-1.0,  0.0, -0.35, 0.35, 0.0,   0.0,
         -0.35,  1.35, 0.0,  0.0,  0.35,-0.35, 0.0,  -1.0,
         -1.0,   0.0,  1.35, 0.35, 0.0,  0.0, -0.35, -0.35,
          0.0,   0.0,  0.35, 1.35, 0.0, -1.0, -0.35, -0.35,
         -0.35,  0.35, 0.0,  0.0,  1.35,-0.35,-1.0,   0.0,
          0.35, -0.35, 0.0, -1.0, -0.35, 1.35, 0.0,   0.0,
          0.0,   0.0, -0.35,-0.35,-1.0,  0.0,  1.35,  0.35,
          0.0,  -1.0, -0.35,-0.35, 0.0,  0.0,  0.35,  1.35
      };
      expected *= EA_l1;
      //! \clang-format on
      const auto flattend_K = sec21::flat_matrix(K);
      REQUIRE(std::size(flattend_K) == std::size(expected));
      REQUIRE(approx_equal(flattend_K, expected, kDivergence));
   }
   SECTION("global stiffness matrix without supports")
   {
      using namespace sec21::structural_analysis::impl;

      std::vector<bool> mask;
      support_mask(std::begin(sys.nodes), std::end(sys.nodes), std::back_inserter(mask));

      auto lookup = make_lookup(sys, Row{0});
      decltype(lookup) supported_rows;
      decltype(lookup) not_supported_rows;

      partition_lookup(
         std::begin(lookup), 
         std::end(lookup),
         std::begin(mask), 
         std::end(mask),
         std::back_inserter(supported_rows),
         std::back_inserter(not_supported_rows));

      const auto supported_cols = impl::row_to_col(std::begin(supported_rows), std::end(supported_rows));

      auto K = impl::global_stiffness_matrix(sys);
      auto K_without_supports = impl::remove_from_matrix(K, supported_rows, supported_cols);
      REQUIRE(K_without_supports.size1() == 5);
      REQUIRE(K_without_supports.size2() == 5);
      // clang-format off
      //              |  1.35, -0.35, -1.0,  0.0, -0.35, |   | u1 |   | Fx1 |
      //              | -0.35,  1.35,  0.0,  0.0,  0.35, |   | v1 |   | Fy1 |
      // 2.8 * 10^3 * | -1.0,   0.0,   1.35  0.35, 0.0,  | * | u2 | = | Fx2 |
      //              |  0.0,   0.0,   0.35, 1.35, 0.0,  |   | v2 |   | Fy2 |
      //              | -0.35,  0.35,  0.0,  0.0,  1.35  |   | u3 |   | Fx3 |
      auto expected = std::valarray{
          1.35, -0.35, -1.0,  0.0, -0.35,
         -0.35,  1.35,  0.0,  0.0,  0.35,
         -1.0,   0.0,   1.35, 0.35, 0.0,
          0.0,   0.0,   0.35, 1.35, 0.0,
         -0.35,  0.35,  0.0,  0.0,  1.35
      };
      expected *= EA_l1;
      // clang-format on
      const auto flattend_K = sec21::flat_matrix(K_without_supports);
      REQUIRE(std::size(flattend_K) == std::size(expected));      
      REQUIRE(approx_equal(flattend_K, expected, kDivergence));
   }
   SECTION("K strich")
   {
      using namespace sec21::structural_analysis::impl;

      std::vector<bool> mask;
      support_mask(std::begin(sys.nodes), std::end(sys.nodes), std::back_inserter(mask));

      auto lookup = make_lookup(sys, Row{0});
      decltype(lookup) supported_rows;
      decltype(lookup) not_supported_rows;

      partition_lookup(
         std::begin(lookup), 
         std::end(lookup),
         std::begin(mask), 
         std::end(mask),
         std::back_inserter(supported_rows),
         std::back_inserter(not_supported_rows));

      const auto supported_cols = impl::row_to_col(std::begin(supported_rows), std::end(supported_rows));

      auto K = impl::global_stiffness_matrix(sys);
      auto K_strich = impl::remove_from_matrix(K, not_supported_rows, supported_cols);
      REQUIRE(K_strich.size1() == 3);
      REQUIRE(K_strich.size2() == 5);
      // clang-format off
      //                                                       | u1 |     
      //              |  0.35, -0.35,  0.0,  -1.0,  -0.35, |   | v1 |   | Fy3 | 
      // 2.8 * 10^3 * |  0.0,   0.0,  -0.35, -0.35, -1.0,  | * | u2 | = | Fx4 |  
      //              |  0.0,  -1.0,  -0.35  -0.35,  0.0   |   | v2 |   | Fy4 |
      //                                                       | u3 |
      auto expected = std::valarray{
          0.35, -0.35, 0.0,  -1.0,  -0.35,
          0.0,   0.0, -0.35, -0.35, -1.0,
          0.0,  -1.0, -0.35, -0.35,  0.0
      };
      expected *= EA_l1;
      // clang-format on
      REQUIRE(approx_equal(sec21::flat_matrix(K_strich), expected, kDivergence));
   }
   SECTION("solve")
   {
      using namespace sec21;

      loadcase_t lf1;
      lf1.node_load.emplace_back(2, loadcase_t::load_t{{10.0_kN, -10.0_kN}});

      auto [success, result] = solve(sys, lf1);
      REQUIRE(success == true);

      // auto it = result.nodes.find(1);
      // REQUIRE(it != std::end(result.nodes));

      // unit: newton [N]
      REQUIRE(X(result.node[n1.value()].support_reaction).value() == Approx(0.0));
      REQUIRE(Y(result.node[n1.value()].support_reaction).value() == Approx(0.0));
      REQUIRE(X(result.node[n2.value()].support_reaction).value() == Approx(0.0));
      REQUIRE(Y(result.node[n2.value()].support_reaction).value() == Approx(0.0));
      REQUIRE(X(result.node[n3.value()].support_reaction).value() == Approx(0.0));
      REQUIRE(Y(result.node[n3.value()].support_reaction).value() == Approx(20'000.0));
      REQUIRE(X(result.node[n4.value()].support_reaction).value() == Approx(-10'000.0));
      REQUIRE(Y(result.node[n4.value()].support_reaction).value() == Approx(-10'000.0));

      // unit: millimeter [mm]
      REQUIRE(X(result.node[n1.value()].displacement).value() == Approx(0.86).epsilon(kDivergence));
      REQUIRE(Y(result.node[n1.value()].displacement).value() == Approx(0.178571));
      REQUIRE(X(result.node[n2.value()].displacement).value() == Approx(1.04079));
      REQUIRE(Y(result.node[n2.value()].displacement).value() == Approx(-0.535714));
      REQUIRE(X(result.node[n3.value()].displacement).value() == Approx(0.178571));
      REQUIRE(Y(result.node[n3.value()].displacement).value() == Approx(0.0));
      REQUIRE(X(result.node[n4.value()].displacement).value() == Approx(0.0));
      REQUIRE(Y(result.node[n4.value()].displacement).value() == Approx(0.0));

      // unit: newton [N])
      REQUIRE(result.member[m1.value()].normal_force.value() == Approx(5'000));
      REQUIRE(result.member[m2.value()].normal_force.value() == Approx(-15'000));
      REQUIRE(result.member[m3.value()].normal_force.value() == Approx(5'000));
      REQUIRE(result.member[m4.value()].normal_force.value() == Approx(5'000));
      REQUIRE(result.member[m5.value()].normal_force.value() == Approx(7'071.0678118655));
      REQUIRE(result.member[m6.value()].normal_force.value() == Approx(-7'071.0678118655));

      {
         std::ofstream ofs{"output_example_1_result.json"};
         nlohmann::json tmp = result;
         ofs << std::setw(4) << tmp;
      }      
   }
}
TEST_CASE("example 1.1 with different node id's (N1 switched with N3 from example 1)", "[sec21][structural_analysis][space_truss]")
{
   using namespace sec21::structural_analysis;
   using namespace sec21::units::literals;

   auto sys = space_truss{};
   using precision_t = decltype(sys)::precision_t;

   using precision_t = decltype(sys)::precision_t;
   using node_t = decltype(sys)::node_t;
   using support_t = node_t::global_support_t;

   auto n1 = add_node(sys, {1, {3.0, 0.0}, support_t{ false, true }});
   auto n2 = add_node(sys, {2, {3.0, 3.0}});
   auto n3 = add_node(sys, {3, {0.0, 3.0}});
   auto n4 = add_node(sys, {4, {0.0, 0.0}, support_t{ true, true }});

   REQUIRE(static_cast<bool>(n1) == true);
   REQUIRE(static_cast<bool>(n2) == true);
   REQUIRE(static_cast<bool>(n3) == true);
   REQUIRE(static_cast<bool>(n4) == true);

   auto m1 = add_member(sys, 3, 2, {1, 0.004, 21'000'000});
   auto m2 = add_member(sys, 1, 2, {2, 0.004, 21'000'000});
   auto m3 = add_member(sys, 4, 1, {3, 0.004, 21'000'000});
   auto m4 = add_member(sys, 4, 3, {4, 0.004, 21'000'000});
   auto m5 = add_member(sys, 4, 2, {5, 0.004, 21'000'000});
   auto m6 = add_member(sys, 1, 3, {6, 0.004, 21'000'000});

   REQUIRE(static_cast<bool>(m1) == true);
   REQUIRE(static_cast<bool>(m2) == true);
   REQUIRE(static_cast<bool>(m3) == true);
   REQUIRE(static_cast<bool>(m4) == true);
   REQUIRE(static_cast<bool>(m5) == true);
   REQUIRE(static_cast<bool>(m6) == true);

   SECTION("test the geometry of the system")
   {
      REQUIRE(impl::length(sys, m1.value()) == 3.0_m);
      REQUIRE(impl::length(sys, m2.value()) == 3.0_m);
      REQUIRE(impl::length(sys, m3.value()) == 3.0_m);
      REQUIRE(impl::length(sys, m4.value()) == 3.0_m);
      REQUIRE(impl::length(sys, m5.value()).value() == Approx(4.24264)); //_m);
      REQUIRE(impl::length(sys, m6.value()).value() == Approx(4.24264)); //_m);

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

      std::vector<bool> mask;
      support_mask(std::begin(sys.nodes), std::end(sys.nodes), std::back_inserter(mask));

      decltype(lookup) supported_nodes;
      decltype(lookup) not_supported_nodes;

      partition_lookup(
         std::begin(lookup), 
         std::end(lookup),
         std::begin(mask), 
         std::end(mask),
         std::back_inserter(supported_nodes),
         std::back_inserter(not_supported_nodes));

      const auto expected_rows = std::vector{ Row{1}, Row{6}, Row{7} };

      REQUIRE(std::size(supported_nodes) == std::size(expected_rows));
      REQUIRE(std::equal(std::begin(supported_nodes), std::end(supported_nodes), std::begin(expected_rows)));
   }
   SECTION("coincidence matrix from member 1")
   {
      auto Z = impl::coincidence_matrix(sys, m1.value());
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
   SECTION("coincidence matrix from member 2")
   {
      auto Z = impl::coincidence_matrix(sys, m2.value());
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
   SECTION("coincidence matrix from member 3")
   {
      auto Z = impl::coincidence_matrix(sys, m3.value());
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
   SECTION("coincidence matrix from member 4")
   {
      auto Z = impl::coincidence_matrix(sys, m4.value());
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
   SECTION("coincidence matrix from member 5")
   {
      auto Z = impl::coincidence_matrix(sys, m5.value());
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
   SECTION("coincidence matrix from member 6")
   {
      auto Z = impl::coincidence_matrix(sys, m6.value());
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
      using namespace sec21;

      loadcase<decltype(sys)> lf1;
      lf1.node_load.emplace_back(2, loadcase<decltype(sys)>::load_t{{10.0_kN, -10.0_kN}});

      auto [success, result] = solve(sys, lf1);
      REQUIRE(success == true);

      // unit: newton [N]
      REQUIRE(X(result.node[n1.value()].support_reaction).value() == Approx(0.0));
      REQUIRE(Y(result.node[n1.value()].support_reaction).value() == Approx(20'000.0));
      REQUIRE(X(result.node[n2.value()].support_reaction).value() == Approx(0.0));
      REQUIRE(Y(result.node[n2.value()].support_reaction).value() == Approx(0.0));
      REQUIRE(X(result.node[n3.value()].support_reaction).value() == Approx(0.0));
      REQUIRE(Y(result.node[n3.value()].support_reaction).value() == Approx(0.0));
      REQUIRE(X(result.node[n4.value()].support_reaction).value() == Approx(-10'000.0));
      REQUIRE(Y(result.node[n4.value()].support_reaction).value() == Approx(-10'000.0));

      // unit: millimeter [mm]
      REQUIRE(X(result.node[n1.value()].displacement).value() == Approx(0.18).epsilon(kDivergence));
      REQUIRE(Y(result.node[n1.value()].displacement).value() == Approx(0.0));
      REQUIRE(X(result.node[n2.value()].displacement).value() == Approx(1.04).epsilon(kDivergence));
      REQUIRE(Y(result.node[n2.value()].displacement).value() == Approx(-0.54).epsilon(kDivergence));
      REQUIRE(X(result.node[n3.value()].displacement).value() == Approx(0.86).epsilon(kDivergence));
      REQUIRE(Y(result.node[n3.value()].displacement).value() == Approx(0.18).epsilon(kDivergence));
      REQUIRE(X(result.node[n4.value()].displacement).value() == Approx(0.0));
      REQUIRE(Y(result.node[n4.value()].displacement).value() == Approx(0.0));

      // unit: newton [N])
      REQUIRE(result.member[m1.value()].normal_force.value() == Approx(5'000));
      REQUIRE(result.member[m2.value()].normal_force.value() == Approx(-15'000));
      REQUIRE(result.member[m3.value()].normal_force.value() == Approx(5'000));
      REQUIRE(result.member[m4.value()].normal_force.value() == Approx(5'000));
      REQUIRE(result.member[m5.value()].normal_force.value() == Approx(7'071).epsilon(kDivergence));
      REQUIRE(result.member[m6.value()].normal_force.value() == Approx(-7'071).epsilon(kDivergence));
   }
}
TEST_CASE("example system 1.2 with half load from example 1.0", "[sec21][structural_analysis][space_truss]")
{
   using namespace sec21::structural_analysis;
   using namespace sec21::units::literals;

   auto sys = space_truss{};

   using precision_t = decltype(sys)::precision_t;
   using node_t = decltype(sys)::node_t;
   using support_t = node_t::global_support_t;

   auto n1 = add_node(sys, {1, {0.0, 3.0}});
   auto n2 = add_node(sys, {2, {3.0, 3.0}});
   auto n3 = add_node(sys, {3, {3.0, 0.0}, support_t{ false, true }});
   auto n4 = add_node(sys, {4, {0.0, 0.0}, support_t{ true, true }});

   REQUIRE(static_cast<bool>(n1) == true);
   REQUIRE(static_cast<bool>(n2) == true);
   REQUIRE(static_cast<bool>(n3) == true);
   REQUIRE(static_cast<bool>(n4) == true);

   // constexpr auto E = 210.0_kPa;
   // constexpr auto A = unit::square_meter<double>(0.004);
   auto m1 = add_member(sys, 1, 2, {1, 0.004, 21'000'000});
   auto m2 = add_member(sys, 3, 2, {2, 0.004, 21'000'000});
   auto m3 = add_member(sys, 4, 3, {3, 0.004, 21'000'000});
   auto m4 = add_member(sys, 4, 1, {4, 0.004, 21'000'000});
   auto m5 = add_member(sys, 4, 2, {5, 0.004, 21'000'000});
   auto m6 = add_member(sys, 3, 1, {6, 0.004, 21'000'000});

   REQUIRE(static_cast<bool>(m1) == true);
   REQUIRE(static_cast<bool>(m2) == true);
   REQUIRE(static_cast<bool>(m3) == true);
   REQUIRE(static_cast<bool>(m4) == true);
   REQUIRE(static_cast<bool>(m5) == true);
   REQUIRE(static_cast<bool>(m6) == true);

   SECTION("test the geometry of the system")
   {
      REQUIRE(impl::length(sys, m1.value()) == 3.0_m);
      REQUIRE(impl::length(sys, m2.value()) == 3.0_m);
      REQUIRE(impl::length(sys, m3.value()) == 3.0_m);
      REQUIRE(impl::length(sys, m4.value()) == 3.0_m);
      REQUIRE(impl::length(sys, m5.value()).value() == Approx(4.24264).epsilon(kDivergence)); //_m);
      REQUIRE(impl::length(sys, m6.value()).value() == Approx(4.24264).epsilon(kDivergence)); //_m);

      namespace bmc = boost::math::constants;
      const auto fourth_pi{bmc::half_pi<precision_t>() * 0.5};

      REQUIRE(impl::angle_to_x_axis(sys, m1.value()) == Approx(0.0));
      REQUIRE(impl::angle_to_x_axis(sys, m2.value()) == Approx(bmc::half_pi<precision_t>()));
      REQUIRE(impl::angle_to_x_axis(sys, m3.value()) == Approx(0.0));
      REQUIRE(impl::angle_to_x_axis(sys, m4.value()) == Approx(bmc::half_pi<precision_t>()));
      REQUIRE(impl::angle_to_x_axis(sys, m5.value()) == Approx(fourth_pi));
      REQUIRE(impl::angle_to_x_axis(sys, m6.value()) == Approx(-fourth_pi));
   }
   SECTION("solve")
   {
      using namespace sec21;

      loadcase<decltype(sys)> lf1{};
      lf1.node_load.emplace_back(2, loadcase<decltype(sys)>::load_t{{5.0_kN, -5.0_kN}});

      auto [success, result] = solve(sys, lf1);
      REQUIRE(success == true);

      // unit: newton [N]
      REQUIRE(X(result.node[n1.value()].support_reaction).value() == Approx(0.0));
      REQUIRE(Y(result.node[n1.value()].support_reaction).value() == Approx(0.0));
      REQUIRE(X(result.node[n2.value()].support_reaction).value() == Approx(0.0));
      REQUIRE(Y(result.node[n2.value()].support_reaction).value() == Approx(0.0));
      REQUIRE(X(result.node[n3.value()].support_reaction).value() == Approx(0.0));
      REQUIRE(Y(result.node[n3.value()].support_reaction).value() == Approx(10'000.0));
      REQUIRE(X(result.node[n4.value()].support_reaction).value() == Approx(-5'000.0));
      REQUIRE(Y(result.node[n4.value()].support_reaction).value() == Approx(-5'000.0));

      // unit: millimeter [mm]
      REQUIRE(X(result.node[n1.value()].displacement).value() == Approx(0.43).epsilon(kDivergence));
      REQUIRE(Y(result.node[n1.value()].displacement).value() == Approx(0.09).epsilon(kDivergence));
      REQUIRE(X(result.node[n2.value()].displacement).value() == Approx(0.52).epsilon(kDivergence));
      REQUIRE(Y(result.node[n2.value()].displacement).value() == Approx(-0.27).epsilon(kDivergence));
      REQUIRE(X(result.node[n3.value()].displacement).value() == Approx(0.09).epsilon(kDivergence));
      REQUIRE(Y(result.node[n3.value()].displacement).value() == Approx(0.0));
      REQUIRE(X(result.node[n4.value()].displacement).value() == Approx(0.0));
      REQUIRE(Y(result.node[n4.value()].displacement).value() == Approx(0.0));

      // unit: newton [N])
      REQUIRE(result.member[m1.value()].normal_force.value() == Approx(2'500));
      REQUIRE(result.member[m2.value()].normal_force.value() == Approx(-7'500));
      REQUIRE(result.member[m3.value()].normal_force.value() == Approx(2'500));
      REQUIRE(result.member[m4.value()].normal_force.value() == Approx(2'500));
      REQUIRE(result.member[m5.value()].normal_force.value() == Approx(3'535.53).epsilon(kDivergence));
      REQUIRE(result.member[m6.value()].normal_force.value() == Approx(-3'535.53).epsilon(kDivergence));
   }
}
TEST_CASE("example system 1.0 load from json", "[sec21][structural_analysis][space_truss]")
{
   using namespace sec21::structural_analysis;
   using namespace sec21::units::literals;

   auto sys = sec21::load_from_json<space_truss>("example_1.json");

   loadcase<decltype(sys)> lf1{};
   lf1.node_load.emplace_back(2, loadcase<decltype(sys)>::load_t{{10.0_kN, -10.0_kN}});

   auto [success, result] = solve(sys, lf1);
   REQUIRE(success == true);

   {
      std::vector<double> flat_support_reaction{};
      for (auto [k,v] : result.node) 
      {
         std::transform(
            std::begin(v.support_reaction), 
            std::end(v.support_reaction), 
            std::back_inserter(flat_support_reaction),
            [](auto&& e) { return e.value(); });
      }

      // unit: newton [N]
      const auto expected = std::array{ 0.0, 0.0, 0.0, 0.0, 0.0, 20'000.0, -10'000.0, -10'000.0 };
      REQUIRE(approx_equal(flat_support_reaction, expected, kDivergence));
   }
   {
      std::vector<double> flat_displacement{};
      for (auto [k,v] : result.node) 
      {
         std::transform(
            std::begin(v.displacement), 
            std::end(v.displacement), 
            std::back_inserter(flat_displacement),
            [](auto&& e) { return e.value(); });
      }

      // unit: millimeter [mm]
      const auto expected = std::array{ 0.869117, 0.18, 1.049117, -0.54, 0.18, 0.0, 0.0, 0.0 };
      REQUIRE(approx_equal(flat_displacement, expected, kDivergence));
   }
   {
      // unit: newton [N]
      std::vector<double> copied_results{};
      std::transform(
         std::begin(result.member), 
         std::end(result.member), 
         std::back_inserter(copied_results), 
         [](auto&& m) { return m.second.normal_force.value(); });

      const auto expected = std::array{ 5'040.0, -15'120.0, 5'040.0, 5'040.0, 7'127.63635, -7'127.63635 };
      REQUIRE(approx_equal(copied_results, expected, kDivergence));
   }
}
