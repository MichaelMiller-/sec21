#include "approx_equal.h"
#include <catch.hpp>

#include <sec21/access.h>
#include <sec21/file_loader.h>
#include <sec21/numeric/drop.h>
#include <sec21/numeric/flatten.h>
#include <sec21/numeric/ublas_allocator_wrapper.h>
#include <sec21/structural_analysis/loadcase.h>
#include <sec21/structural_analysis/solve.h>
#include <sec21/structural_analysis/node.h>
#include <sec21/structural_analysis/member.h>
#include <sec21/structural_analysis/space_truss.h>
#include <sec21/units.h>
#include <sec21/structural_analysis/solver/backend/eigen.h>

#include <algorithm>
#include <array>
#include <utility>
#include <valarray>
#include <vector>

TEST_CASE("example system 1.0", "[sec21][structural_analysis][space_truss]")
{
   //! \todo possible to implement a strong_type (percent) with a value range from 0...100%
   constexpr auto kDivergence{0.02};

   using namespace sec21;
   using namespace sec21::structural_analysis;
   using namespace sec21::units::literals;

   using member_t = member<int, double>;
   using node_t = node<2, int, double>;
   using space_truss_t = space_truss<node_t, member_t>;
   using support_t = node_t::global_support_t;

   auto sys = space_truss_t{};

   static_assert(is_space_truss<decltype(sys)>::value);
   static_assert(is_space_truss_2D<decltype(sys)>::value);

   using precision_t = decltype(sys)::precision_t;
   using loadcase_t = loadcase<decltype(sys)>;

   auto n1 = add_node(sys, node_t{1u, {0.0, 3.0}});
   auto n2 = add_node(sys, node_t{2u, {3.0, 3.0}});
   auto n3 = add_node(sys, node_t{3u, {3.0, 0.0}, support_t{false, true}});
   auto n4 = add_node(sys, node_t{4u, {0.0, 0.0}, support_t{true, true}});

   REQUIRE(static_cast<bool>(n1) == true);
   REQUIRE(static_cast<bool>(n2) == true);
   REQUIRE(static_cast<bool>(n3) == true);
   REQUIRE(static_cast<bool>(n4) == true);

   //! \todo use units
   // constexpr auto E = 21'000'000.0_kPa;
   // constexpr auto A = unit::square_meter<double>(0.004_sq_m);
   auto m1 = add_member(sys, 1, 2, member_t{1u, "", {}, "", "", 0.004, 21'000'000});
   auto m2 = add_member(sys, 3, 2, member_t{2u, "", {}, "", "", 0.004, 21'000'000});
   auto m3 = add_member(sys, 4, 3, member_t{3u, "", {}, "", "", 0.004, 21'000'000});
   auto m4 = add_member(sys, 4, 1, member_t{4u, "", {}, "", "", 0.004, 21'000'000});
   auto m5 = add_member(sys, 4, 2, member_t{5u, "", {}, "", "", 0.004, 21'000'000});
   auto m6 = add_member(sys, 3, 1, member_t{6u, "", {}, "", "", 0.004, 21'000'000});

   const auto EA_l1 = 28'000.0;
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
      auto lookup = structural_analysis::impl::make_lookup(sys, row{1});
      const auto expected = std::array{row{1}, row{2}, row{3}, row{4}, row{5}, row{6}, row{7}, row{8}};

      REQUIRE(std::size(lookup) == std::size(expected));
      REQUIRE(std::equal(std::begin(lookup), std::end(lookup), std::begin(expected)));
   }
   SECTION("filter supported nodes from lookup table")
   {
      using namespace sec21::structural_analysis::impl;

      auto lookup = make_lookup(sys, row{0});
      const auto expected = std::array{row{0}, row{1}, row{2}, row{3}, row{4}, row{5}, row{6}, row{7}};

      REQUIRE(std::size(lookup) == std::size(expected));
      REQUIRE(std::equal(std::begin(lookup), std::end(lookup), std::begin(expected)));

      std::vector<bool> mask;
      support_mask(std::begin(sys.nodes), std::end(sys.nodes), std::back_inserter(mask));

      decltype(lookup) supported_nodes;
      decltype(lookup) not_supported_nodes;

      partition_lookup(std::begin(lookup), std::end(lookup), std::begin(mask), std::end(mask),
                       std::back_inserter(supported_nodes), std::back_inserter(not_supported_nodes));

      const auto expected_rows = std::vector{row{5}, row{6}, row{7}};

      REQUIRE(std::size(supported_nodes) == std::size(expected_rows));
      REQUIRE(std::equal(std::begin(supported_nodes), std::end(supported_nodes), std::begin(expected_rows)));
   }
   SECTION("Steifigkeitsbeziehung vom Fachwerkstab 1 in globalen Koordinaten")
   {
      using allocator_t = sec21::numeric::ublas_allocator_wrapper<std::allocator<double>>;
      auto result = impl::steifigkeitsbeziehung_fachwerkstab_globalen_koordinaten<allocator_t>(sys, m1.value());

      REQUIRE(result.size1() == 4);
      REQUIRE(result.size2() == 4);
      // clang-format off
      ///
      /// Stab 1
      /// ------
      /// Fx1            1  0 -1  0      u1 
      /// Fy1 = E*A/l *  0  0  0  0   *  v1
      /// Fx2           -1  0  1  0      u2
      /// Fy2            0  0  0  0      v2
      ///
      auto expected = std::valarray{
         1.0, 0.0,-1.0, 0.0,
         0.0, 0.0, 0.0, 0.0,
        -1.0, 0.0, 1.0, 0.0,
         0.0, 0.0, 0.0, 0.0,
      };
      expected *= EA_l1;
      // clang-format on
      REQUIRE(approx_equal(numeric::flatten(result), expected, kDivergence));
   }
   SECTION("Steifigkeitsbeziehung vom Fachwerkstab 2 in globalen Koordinaten")
   {
      using allocator_t = sec21::numeric::ublas_allocator_wrapper<std::allocator<double>>;
      auto result = impl::steifigkeitsbeziehung_fachwerkstab_globalen_koordinaten<allocator_t>(sys, m2.value());

      REQUIRE(result.size1() == 4);
      REQUIRE(result.size2() == 4);
      // clang-format off
      ///
      /// Stab 2
      /// ------
      /// Fx3            0  0  0  0      u3 
      /// Fy3 = E*A/l *  0  1  0 -1   *  v3
      /// Fx2            0  0  0  0      u2
      /// Fy2            0 -1  0  1      v2
      ///
      auto expected = std::valarray{
         0.0, 0.0, 0.0, 0.0,
         0.0, 1.0, 0.0,-1.0,
         0.0, 0.0, 0.0, 0.0,
         0.0,-1.0, 0.0, 1.0,
      };
      expected *= EA_l1;
      // clang-format on
      REQUIRE(approx_equal(numeric::flatten(result), expected, kDivergence));
   }
   SECTION("Steifigkeitsbeziehung vom Fachwerkstab 3 in globalen Koordinaten")
   {
      using allocator_t = sec21::numeric::ublas_allocator_wrapper<std::allocator<double>>;
      auto result = impl::steifigkeitsbeziehung_fachwerkstab_globalen_koordinaten<allocator_t>(sys, m3.value());

      REQUIRE(result.size1() == 4);
      REQUIRE(result.size2() == 4);
      // clang-format off
      ///
      /// Stab 3
      /// ------
      /// Fx4            1  0 -1  0      u4 
      /// Fy4 = E*A/l *  0  0  0  0   *  v4
      /// Fx3           -1  0  1  0      u3
      /// Fy3            0  0  0  0      v3
      ///
      auto expected = std::valarray{
         1.0, 0.0,-1.0, 0.0,
         0.0, 0.0, 0.0, 0.0,
        -1.0, 0.0, 1.0, 0.0,
         0.0, 0.0, 0.0, 0.0,
      };
      expected *= EA_l1;
      // clang-format on
      REQUIRE(approx_equal(numeric::flatten(result), expected, kDivergence));
   }
   SECTION("Steifigkeitsbeziehung vom Fachwerkstab 4 in globalen Koordinaten")
   {
      using allocator_t = sec21::numeric::ublas_allocator_wrapper<std::allocator<double>>;
      auto result = impl::steifigkeitsbeziehung_fachwerkstab_globalen_koordinaten<allocator_t>(sys, m4.value());

      REQUIRE(result.size1() == 4);
      REQUIRE(result.size2() == 4);
      // clang-format off
      ///
      /// Stab 4
      /// ------
      /// Fx1            0  0  0  0      u1 
      /// Fy1 = E*A/l *  0  1  0  -1  *  v1
      /// Fx4            0  0  0  0      u4
      /// Fy4            0  -1  0  1     v4
      ///
      auto expected = std::valarray{
         0.0, 0.0, 0.0, 0.0,
         0.0, 1.0, 0.0,-1.0,
         0.0, 0.0, 0.0, 0.0,
         0.0,-1.0, 0.0, 1.0,
      };
      expected *= EA_l1;
      // clang-format on
      REQUIRE(approx_equal(numeric::flatten(result), expected, kDivergence));
   }
   SECTION("Steifigkeitsbeziehung vom Fachwerkstab 5 in globalen Koordinaten")
   {
      using allocator_t = sec21::numeric::ublas_allocator_wrapper<std::allocator<double>>;
      auto result = impl::steifigkeitsbeziehung_fachwerkstab_globalen_koordinaten<allocator_t>(sys, m5.value());

      REQUIRE(result.size1() == 4);
      REQUIRE(result.size2() == 4);
      // clang-format off
      auto expected = std::valarray{
          0.5, 0.5,-0.5,-0.5,
          0.5, 0.5,-0.5,-0.5,
         -0.5,-0.5, 0.5, 0.5,
         -0.5,-0.5, 0.5, 0.5
      };
      expected *= EA_l2; // E*A / l
      // clang-format on
      REQUIRE(approx_equal(numeric::flatten(result), expected, kDivergence));
   }
   SECTION("Steifigkeitsbeziehung vom Fachwerkstab 6 in globalen Koordinaten")
   {
      using allocator_t = sec21::numeric::ublas_allocator_wrapper<std::allocator<double>>;
      auto result = impl::steifigkeitsbeziehung_fachwerkstab_globalen_koordinaten<allocator_t>(sys, m6.value());

      REQUIRE(result.size1() == 4);
      REQUIRE(result.size2() == 4);
      // clang-format off
      auto expected = std::valarray{
          0.5,-0.5,-0.5, 0.5,
         -0.5, 0.5, 0.5,-0.5,
         -0.5, 0.5, 0.5,-0.5,
          0.5,-0.5,-0.5, 0.5
      };
      expected *= EA_l2; // E*A / l
      // clang-format on
      REQUIRE(approx_equal(numeric::flatten(result), expected, kDivergence));
   }
   SECTION("coincidence matrix from member 1")
   {
      using allocator_t = sec21::numeric::ublas_allocator_wrapper<std::allocator<double>>;
      auto Z = impl::coincidence_matrix<allocator_t>(sys, m1.value());

      REQUIRE(Z.size1() == 4);
      REQUIRE(Z.size2() == 8);
      // clang-format off
      const auto expected = std::array{
         1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0,
      };
      // clang-format on
      REQUIRE(approx_equal(numeric::flatten(Z), expected, kDivergence));
   }
   SECTION("coincidence matrix from member 2")
   {
      using allocator_t = sec21::numeric::ublas_allocator_wrapper<std::allocator<double>>;
      auto Z = impl::coincidence_matrix<allocator_t>(sys, m2.value());

      REQUIRE(Z.size1() == 4);
      REQUIRE(Z.size2() == 8);
      // clang-format off
      const auto expected = std::array{
         0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0,
         0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0,
      };
      // clang-format on
      REQUIRE(approx_equal(numeric::flatten(Z), expected, kDivergence));
   }
   SECTION("coincidence matrix from member 3")
   {
      using allocator_t = sec21::numeric::ublas_allocator_wrapper<std::allocator<double>>;
      auto Z = impl::coincidence_matrix<allocator_t>(sys, m3.value());

      REQUIRE(Z.size1() == 4);
      REQUIRE(Z.size2() == 8);
      // clang-format off
      const auto expected = std::array{
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0,
         0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0,
      };
      // clang-format on
      REQUIRE(approx_equal(numeric::flatten(Z), expected, kDivergence));
   }
   SECTION("coincidence matrix from member 4")
   {
      using allocator_t = sec21::numeric::ublas_allocator_wrapper<std::allocator<double>>;
      auto Z = impl::coincidence_matrix<allocator_t>(sys, m4.value());

      REQUIRE(Z.size1() == 4);
      REQUIRE(Z.size2() == 8);
      // clang-format off
      const auto expected = std::array{
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0,
         1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      };
      // clang-format on
      REQUIRE(approx_equal(numeric::flatten(Z), expected, kDivergence));
   }
   SECTION("coincidence matrix from member 5")
   {
      using allocator_t = sec21::numeric::ublas_allocator_wrapper<std::allocator<double>>;
      auto Z = impl::coincidence_matrix<allocator_t>(sys, m5.value());

      REQUIRE(Z.size1() == 4);
      REQUIRE(Z.size2() == 8);
      // clang-format off
      const auto expected = std::array{
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0,
         0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0,
      };
      // clang-format on
      REQUIRE(approx_equal(numeric::flatten(Z), expected, kDivergence));
   }
   SECTION("coincidence matrix from member 6")
   {
      using allocator_t = sec21::numeric::ublas_allocator_wrapper<std::allocator<double>>;
      auto Z = impl::coincidence_matrix<allocator_t>(sys, m6.value());

      REQUIRE(Z.size1() == 4);
      REQUIRE(Z.size2() == 8);
      // clang-format off
      const auto expected = std::array{
         0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0,
         1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      };
      // clang-format on
      REQUIRE(approx_equal(numeric::flatten(Z), expected, kDivergence));
   }
   SECTION("stiffness matrix from member 1")
   {
      using allocator_t = sec21::numeric::ublas_allocator_wrapper<std::allocator<double>>;
      auto K = impl::stiffness_matrix<allocator_t>(sys, m1.value());

      REQUIRE(K.size1() == 8);
      REQUIRE(K.size2() == 8);
      // clang-format off
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
      // clang-format on
      const auto flattend_K = numeric::flatten(K);
      REQUIRE(std::size(flattend_K) == std::size(expected));
      REQUIRE(approx_equal(flattend_K, expected, kDivergence));
   }
   SECTION("stiffness matrix from member 2")
   {
      using allocator_t = sec21::numeric::ublas_allocator_wrapper<std::allocator<double>>;
      auto K = impl::stiffness_matrix<allocator_t>(sys, m2.value());

      REQUIRE(K.size1() == 8);
      REQUIRE(K.size2() == 8);
      // clang-format off
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
      // clang-format on
      const auto flattend_K = numeric::flatten(K);
      REQUIRE(std::size(flattend_K) == std::size(expected));
      REQUIRE(approx_equal(flattend_K, expected, kDivergence));
   }
   SECTION("stiffness matrix from member 3")
   {
      using allocator_t = sec21::numeric::ublas_allocator_wrapper<std::allocator<double>>;
      auto K = impl::stiffness_matrix<allocator_t>(sys, m3.value());

      REQUIRE(K.size1() == 8);
      REQUIRE(K.size2() == 8);
      // clang-format off
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
      // clang-format on
      const auto flattend_K = numeric::flatten(K);
      REQUIRE(std::size(flattend_K) == std::size(expected));
      REQUIRE(approx_equal(flattend_K, expected, kDivergence));
   }
   SECTION("stiffness matrix from member 4")
   {
      using allocator_t = sec21::numeric::ublas_allocator_wrapper<std::allocator<double>>;
      auto K = impl::stiffness_matrix<allocator_t>(sys, m4.value());

      REQUIRE(K.size1() == 8);
      REQUIRE(K.size2() == 8);
      // clang-format off
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
      // clang-format on
      const auto flattend_K = numeric::flatten(K);
      REQUIRE(std::size(flattend_K) == std::size(expected));
      REQUIRE(approx_equal(flattend_K, expected, kDivergence));
   }
   SECTION("stiffness matrix from member 5")
   {
      using allocator_t = sec21::numeric::ublas_allocator_wrapper<std::allocator<precision_t>>;
      auto K = impl::stiffness_matrix<allocator_t>(sys, m5.value());

      REQUIRE(K.size1() == 8);
      REQUIRE(K.size2() == 8);
      // clang-format off
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
      // clang-format on
      const auto flattend_K = numeric::flatten(K);
      REQUIRE(std::size(flattend_K) == std::size(expected));
      REQUIRE(approx_equal(flattend_K, expected, kDivergence));
   }
   SECTION("stiffness matrix from member 6")
   {
      using allocator_t = sec21::numeric::ublas_allocator_wrapper<std::allocator<precision_t>>;
      auto K = impl::stiffness_matrix<allocator_t>(sys, m6.value());

      REQUIRE(K.size1() == 8);
      REQUIRE(K.size2() == 8);
      // clang-format off
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
      // clang-format on
      const auto flattend_K = numeric::flatten(K);
      REQUIRE(std::size(flattend_K) == std::size(expected));
      REQUIRE(approx_equal(flattend_K, expected, kDivergence));
   }
   SECTION("global stiffness matrix")
   {
      using allocator_t = sec21::numeric::ublas_allocator_wrapper<std::allocator<precision_t>>;
      auto K = impl::global_stiffness_matrix<allocator_t>(sys);

      REQUIRE(K.size1() == 8);
      REQUIRE(K.size2() == 8);
      // clang-format off
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
      // clang-format on
      const auto flattend_K = numeric::flatten(K);
      REQUIRE(std::size(flattend_K) == std::size(expected));
      REQUIRE(approx_equal(flattend_K, expected, kDivergence));
   }
   SECTION("global stiffness matrix without supports")
   {
      using namespace sec21::structural_analysis::impl;
      using namespace sec21;

      std::vector<bool> mask;
      support_mask(std::begin(sys.nodes), std::end(sys.nodes), std::back_inserter(mask));

      auto lookup = make_lookup(sys, row{0});
      decltype(lookup) supported_rows;
      decltype(lookup) not_supported_rows;

      partition_lookup(std::begin(lookup), std::end(lookup), std::begin(mask), std::end(mask),
                       std::back_inserter(supported_rows), std::back_inserter(not_supported_rows));

      const auto supported_cols = row_to_col(std::begin(supported_rows), std::end(supported_rows));

      using allocator_t = sec21::numeric::ublas_allocator_wrapper<std::allocator<precision_t>>;
      auto K = impl::global_stiffness_matrix<allocator_t>(sys);
      auto K_without_supports = sec21::numeric::drop(sec21::numeric::drop(K, supported_rows), supported_cols);
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
      const auto flattend_K = numeric::flatten(K_without_supports);
      REQUIRE(std::size(flattend_K) == std::size(expected));
      REQUIRE(approx_equal(flattend_K, expected, kDivergence));
   }
   SECTION("K strich")
   {
      using namespace sec21;
      using namespace sec21::structural_analysis::impl;

      std::vector<bool> mask;
      support_mask(std::begin(sys.nodes), std::end(sys.nodes), std::back_inserter(mask));

      auto lookup = make_lookup(sys, row{0});
      decltype(lookup) supported_rows;
      decltype(lookup) not_supported_rows;

      partition_lookup(std::begin(lookup), std::end(lookup), std::begin(mask), std::end(mask),
                       std::back_inserter(supported_rows), std::back_inserter(not_supported_rows));

      const auto supported_cols = row_to_col(std::begin(supported_rows), std::end(supported_rows));

      using allocator_t = sec21::numeric::ublas_allocator_wrapper<std::allocator<precision_t>>;
      auto K = impl::global_stiffness_matrix<allocator_t>(sys);
      auto K_strich = numeric::drop(numeric::drop(K, not_supported_rows), supported_cols);
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
      const auto flattend_K = numeric::flatten(K_strich);
      REQUIRE(approx_equal(flattend_K, expected, kDivergence));
   }
   SECTION("solve")
   {
      using namespace sec21;

      loadcase_t lf1;
      lf1.node_load.emplace_back(2, loadcase_t::load_t{{10.0_kN, -10.0_kN}});

      const auto success = solve<solver::backend::eigen>(sys, lf1);
      REQUIRE(success.has_value() == true);

      const auto result = success.value();

      // unit: newton [N]
      REQUIRE(std::get<0>(result.nodes[2].support_reaction).value() == Approx(0.0).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[2].support_reaction).value() == Approx(20'000).epsilon(kDivergence));
      REQUIRE(std::get<0>(result.nodes[3].support_reaction).value() == Approx(-10'000).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[3].support_reaction).value() == Approx(-10'000).epsilon(kDivergence));

      // unit: millimeter [mm]
      REQUIRE(std::get<0>(result.nodes[0].displacement).value() == Approx(0.86).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[0].displacement).value() == Approx(0.18).epsilon(kDivergence));
      REQUIRE(std::get<0>(result.nodes[1].displacement).value() == Approx(1.04).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[1].displacement).value() == Approx(-0.53).epsilon(kDivergence));
      REQUIRE(std::get<0>(result.nodes[2].displacement).value() == Approx(0.18).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[2].displacement).value() == Approx(0.0).epsilon(kDivergence));
      REQUIRE(std::get<0>(result.nodes[3].displacement).value() == Approx(0.0).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[3].displacement).value() == Approx(0.0).epsilon(kDivergence));

      std::vector<double> flat_member_result{};
      std::transform(std::begin(result.members), std::end(result.members), std::back_inserter(flat_member_result),
                     [](auto&& e) { return e.normal_force.value(); });

      // unit: newton [N])
      REQUIRE(flat_member_result[0] == Approx(5'000));
      REQUIRE(flat_member_result[1] == Approx(-15'000));
      REQUIRE(flat_member_result[2] == Approx(5'000));
      REQUIRE(flat_member_result[3] == Approx(5'000));
      REQUIRE(flat_member_result[4] == Approx(7'071.0678118655));
      REQUIRE(flat_member_result[5] == Approx(-7'071.0678118655));
   }
   SECTION("solve with eigen backend")
   {
      using namespace sec21;

      loadcase_t lf1;
      lf1.node_load.emplace_back(2, loadcase_t::load_t{{10.0_kN, -10.0_kN}});

      const auto success = solve<solver::backend::eigen>(sys, lf1);
      REQUIRE(success.has_value() == true);

      const auto result = success.value();

      // unit: newton [N]
      REQUIRE(std::get<0>(result.nodes[2].support_reaction).value() == Approx(0.0).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[2].support_reaction).value() == Approx(20'000).epsilon(kDivergence));
      REQUIRE(std::get<0>(result.nodes[3].support_reaction).value() == Approx(-10'000).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[3].support_reaction).value() == Approx(-10'000).epsilon(kDivergence));

      // unit: millimeter [mm]
      REQUIRE(std::get<0>(result.nodes[0].displacement).value() == Approx(0.86).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[0].displacement).value() == Approx(0.18).epsilon(kDivergence));
      REQUIRE(std::get<0>(result.nodes[1].displacement).value() == Approx(1.04).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[1].displacement).value() == Approx(-0.53).epsilon(kDivergence));
      REQUIRE(std::get<0>(result.nodes[2].displacement).value() == Approx(0.18).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[2].displacement).value() == Approx(0.0).epsilon(kDivergence));
      REQUIRE(std::get<0>(result.nodes[3].displacement).value() == Approx(0.0).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[3].displacement).value() == Approx(0.0).epsilon(kDivergence));

      std::vector<double> flat_member_result{};
      std::transform(std::begin(result.members), std::end(result.members), std::back_inserter(flat_member_result),
                     [](auto&& e) { return e.normal_force.value(); });

      // unit: newton [N])
      REQUIRE(flat_member_result[0] == Approx(5'000));
      REQUIRE(flat_member_result[1] == Approx(-15'000));
      REQUIRE(flat_member_result[2] == Approx(5'000));
      REQUIRE(flat_member_result[3] == Approx(5'000));
      REQUIRE(flat_member_result[4] == Approx(7'071.0678118655));
      REQUIRE(flat_member_result[5] == Approx(-7'071.0678118655));
   }
}