#include "approx_equal.h"
#include <catch.hpp>

#include <sec21/access.h>
#include <sec21/numeric/drop.h>
#include <sec21/numeric/flatten.h>
#include <sec21/numeric/ublas_allocator_wrapper.h>
#include <sec21/structural_analysis/loadcase.h>
#include <sec21/structural_analysis/constants.h>
#include <sec21/structural_analysis/solve.h>
#include <sec21/structural_analysis/node.h>
#include <sec21/structural_analysis/member.h>
#include <sec21/structural_analysis/space_truss.h>
#include <sec21/units.h>
#include <sec21/structural_analysis/solver/backend/eigen.h>

#include <array>
#include <valarray>
#include <vector>

TEST_CASE("example system 1.0", "[sec21][structural_analysis][space_truss]")
{
   //! \todo possible to implement a strong_type (percent) with a value range from 0...100%
   constexpr auto kDivergence{0.02};

   using namespace sec21;
   using namespace sec21::structural_analysis;
   using namespace sec21::units::literals;

   using precision_t = double;
   using member_t = member<int, precision_t>;
   using node_t = node<2, int, precision_t>;
   using space_truss_t = space_truss<node_t, member_t>;
   using support_t = node_t::global_support_t;

   auto sys = space_truss_t{};

   static_assert(is_space_truss<decltype(sys)>::value);
   static_assert(is_space_truss_2D<decltype(sys)>::value);

   using loadcase_t = loadcase<decltype(sys)>;

   auto n1 = add_node(sys, node_t{1u, {0.0, 3.0}});
   auto n2 = add_node(sys, node_t{2u, {3.0, 3.0}});
   auto n3 = add_node(sys, node_t{3u, {3.0, 0.0}, support_t{false, true}});
   auto n4 = add_node(sys, node_t{4u, {0.0, 0.0}, support_t{true, true}});

   REQUIRE(n1 == 1);
   REQUIRE(n2 == 2);
   REQUIRE(n3 == 3);
   REQUIRE(n4 == 4);

   //! \todo use units
   // constexpr auto E = 21'000'000.0_kPa;
   // constexpr auto A = unit::square_meter<double>(0.004_sq_m);
   auto m1 = add_member(sys, 1, 2, member_t{1u, "", {}, "", "", 0.004, 21'000'000});
   auto m2 = add_member(sys, 3, 2, member_t{2u, "", {}, "", "", 0.004, 21'000'000});
   auto m3 = add_member(sys, 4, 3, member_t{3u, "", {}, "", "", 0.004, 21'000'000});
   auto m4 = add_member(sys, 4, 1, member_t{4u, "", {}, "", "", 0.004, 21'000'000});
   auto m5 = add_member(sys, 4, 2, member_t{5u, "", {}, "", "", 0.004, 21'000'000});
   auto m6 = add_member(sys, 3, 1, member_t{6u, "", {}, "", "", 0.004, 21'000'000});

   const precision_t EA_l1 = 28'000.0;
   const precision_t EA_l2 = 19'798.98987;

   REQUIRE(m1 == 1);
   REQUIRE(m2 == 2);
   REQUIRE(m3 == 3);
   REQUIRE(m4 == 4);
   REQUIRE(m5 == 5);
   REQUIRE(m6 == 6);

   SECTION("test the geometry of the system")
   {
      REQUIRE(impl::length(sys, m1) == 3.0_m);
      REQUIRE(impl::length(sys, m2) == 3.0_m);
      REQUIRE(impl::length(sys, m3) == 3.0_m);
      REQUIRE(impl::length(sys, m4) == 3.0_m);
      REQUIRE(impl::length(sys, m5).value() == Catch::Approx(4.24264)); //_m);
      REQUIRE(impl::length(sys, m6).value() == Catch::Approx(4.24264)); //_m);
   }
   SECTION("create lookup table")
   {
      auto lookup = structural_analysis::impl::make_lookup(sys, row{1});
      const auto expected = std::array{row{1}, row{2}, row{3}, row{4}, row{5}, row{6}, row{7}, row{8}};

      REQUIRE(std::size(lookup) == std::size(expected));
      REQUIRE(std::equal(std::begin(lookup), std::end(lookup), std::begin(expected)));
   }
   SECTION("Steifigkeitsbeziehung vom Fachwerkstab 1 in globalen Koordinaten")
   {
      using allocator_t = sec21::numeric::ublas_allocator_wrapper<std::allocator<precision_t>>;
      auto result = impl::steifigkeitsbeziehung_fachwerkstab_globalen_koordinaten<allocator_t>(sys, m1);

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
      auto expected = std::valarray<precision_t>{
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
      using allocator_t = sec21::numeric::ublas_allocator_wrapper<std::allocator<precision_t>>;
      auto result = impl::steifigkeitsbeziehung_fachwerkstab_globalen_koordinaten<allocator_t>(sys, m2);

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
      auto expected = std::valarray<precision_t>{
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
      using allocator_t = sec21::numeric::ublas_allocator_wrapper<std::allocator<precision_t>>;
      auto result = impl::steifigkeitsbeziehung_fachwerkstab_globalen_koordinaten<allocator_t>(sys, m3);

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
      auto expected = std::valarray<precision_t>{
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
      using allocator_t = sec21::numeric::ublas_allocator_wrapper<std::allocator<precision_t>>;
      auto result = impl::steifigkeitsbeziehung_fachwerkstab_globalen_koordinaten<allocator_t>(sys, m4);

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
      auto expected = std::valarray<precision_t>{
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
      using allocator_t = sec21::numeric::ublas_allocator_wrapper<std::allocator<precision_t>>;
      auto result = impl::steifigkeitsbeziehung_fachwerkstab_globalen_koordinaten<allocator_t>(sys, m5);

      REQUIRE(result.size1() == 4);
      REQUIRE(result.size2() == 4);
      // clang-format off
      auto expected = std::valarray<precision_t>{
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
      using allocator_t = sec21::numeric::ublas_allocator_wrapper<std::allocator<precision_t>>;
      auto result = impl::steifigkeitsbeziehung_fachwerkstab_globalen_koordinaten<allocator_t>(sys, m6);

      REQUIRE(result.size1() == 4);
      REQUIRE(result.size2() == 4);
      // clang-format off
      auto expected = std::valarray<precision_t>{
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
      using allocator_t = sec21::numeric::ublas_allocator_wrapper<std::allocator<precision_t>>;
      auto Z = impl::coincidence_matrix<allocator_t>(sys, m1);

      REQUIRE(Z.size1() == 4);
      REQUIRE(Z.size2() == 8);
      // clang-format off
      const auto expected = std::array<precision_t, 4*8>{
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
      using allocator_t = sec21::numeric::ublas_allocator_wrapper<std::allocator<precision_t>>;
      auto Z = impl::coincidence_matrix<allocator_t>(sys, m2);

      REQUIRE(Z.size1() == 4);
      REQUIRE(Z.size2() == 8);
      // clang-format off
      const auto expected = std::array<precision_t, 4*8>{
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
      using allocator_t = sec21::numeric::ublas_allocator_wrapper<std::allocator<precision_t>>;
      auto Z = impl::coincidence_matrix<allocator_t>(sys, m3);

      REQUIRE(Z.size1() == 4);
      REQUIRE(Z.size2() == 8);
      // clang-format off
      const auto expected = std::array<precision_t, 4*8>{
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
      using allocator_t = sec21::numeric::ublas_allocator_wrapper<std::allocator<precision_t>>;
      auto Z = impl::coincidence_matrix<allocator_t>(sys, m4);

      REQUIRE(Z.size1() == 4);
      REQUIRE(Z.size2() == 8);
      // clang-format off
      const auto expected = std::array<precision_t, 4*8>{
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
      using allocator_t = sec21::numeric::ublas_allocator_wrapper<std::allocator<precision_t>>;
      auto Z = impl::coincidence_matrix<allocator_t>(sys, m5);

      REQUIRE(Z.size1() == 4);
      REQUIRE(Z.size2() == 8);
      // clang-format off
      const auto expected = std::array<precision_t, 4*8>{
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
      using allocator_t = sec21::numeric::ublas_allocator_wrapper<std::allocator<precision_t>>;
      auto Z = impl::coincidence_matrix<allocator_t>(sys, m6);

      REQUIRE(Z.size1() == 4);
      REQUIRE(Z.size2() == 8);
      // clang-format off
      const auto expected = std::array<precision_t, 4*8>{
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
      using allocator_t = sec21::numeric::ublas_allocator_wrapper<std::allocator<precision_t>>;
      auto K = impl::stiffness_matrix<allocator_t>(sys, m1);

      REQUIRE(K.size1() == 8);
      REQUIRE(K.size2() == 8);
      // clang-format off
      auto expected = std::valarray<precision_t>{
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
      using allocator_t = sec21::numeric::ublas_allocator_wrapper<std::allocator<precision_t>>;
      auto K = impl::stiffness_matrix<allocator_t>(sys, m2);

      REQUIRE(K.size1() == 8);
      REQUIRE(K.size2() == 8);
      // clang-format off
      auto expected = std::valarray<precision_t>{
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
      using allocator_t = sec21::numeric::ublas_allocator_wrapper<std::allocator<precision_t>>;
      auto K = impl::stiffness_matrix<allocator_t>(sys, m3);

      REQUIRE(K.size1() == 8);
      REQUIRE(K.size2() == 8);
      // clang-format off
      auto expected = std::valarray<precision_t>{
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
      using allocator_t = sec21::numeric::ublas_allocator_wrapper<std::allocator<precision_t>>;
      auto K = impl::stiffness_matrix<allocator_t>(sys, m4);

      REQUIRE(K.size1() == 8);
      REQUIRE(K.size2() == 8);
      // clang-format off
      auto expected = std::valarray<precision_t>{
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
      auto K = impl::stiffness_matrix<allocator_t>(sys, m5);

      REQUIRE(K.size1() == 8);
      REQUIRE(K.size2() == 8);
      // clang-format off
      auto expected = std::valarray<precision_t>{
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
      auto K = impl::stiffness_matrix<allocator_t>(sys, m6);

      REQUIRE(K.size1() == 8);
      REQUIRE(K.size2() == 8);
      // clang-format off
      auto expected = std::valarray<precision_t>{
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
      auto expected = std::valarray<precision_t>{
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
   SECTION("solve")
   {
      using namespace sec21;

      loadcase_t lf1;
      lf1.node_load.emplace_back(2, loadcase_t::load_t{{10.0_kN, -10.0_kN}});

      const auto success = solve<solver::backend::eigen>(sys, lf1);
      REQUIRE(success.has_value() == true);

      const auto result = success.value();

      // unit: newton [N]
      REQUIRE(std::get<0>(result.nodes[2].support_reaction).value() == Catch::Approx(0.0).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[2].support_reaction).value() == Catch::Approx(20'000).epsilon(kDivergence));
      REQUIRE(std::get<0>(result.nodes[3].support_reaction).value() == Catch::Approx(-10'000).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[3].support_reaction).value() == Catch::Approx(-10'000).epsilon(kDivergence));

      // unit: millimeter [mm]
      REQUIRE(std::get<0>(result.nodes[0].displacement).value() == Catch::Approx(0.86).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[0].displacement).value() == Catch::Approx(0.18).epsilon(kDivergence));
      REQUIRE(std::get<0>(result.nodes[1].displacement).value() == Catch::Approx(1.04).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[1].displacement).value() == Catch::Approx(-0.53).epsilon(kDivergence));
      REQUIRE(std::get<0>(result.nodes[2].displacement).value() == Catch::Approx(0.18).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[2].displacement).value() == Catch::Approx(0.0).epsilon(kDivergence));
      REQUIRE(std::get<0>(result.nodes[3].displacement).value() == Catch::Approx(0.0).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[3].displacement).value() == Catch::Approx(0.0).epsilon(kDivergence));

      std::vector<double> flat_member_result{};
      std::transform(std::begin(result.members), std::end(result.members), std::back_inserter(flat_member_result),
                     [](auto&& e) { return e.normal_force.value(); });

      // unit: newton [N])
      REQUIRE(flat_member_result[0] == Catch::Approx(5'000));
      REQUIRE(flat_member_result[1] == Catch::Approx(-15'000));
      REQUIRE(flat_member_result[2] == Catch::Approx(5'000));
      REQUIRE(flat_member_result[3] == Catch::Approx(5'000));
      REQUIRE(flat_member_result[4] == Catch::Approx(7'071.0678118655));
      REQUIRE(flat_member_result[5] == Catch::Approx(-7'071.0678118655));
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
      REQUIRE(std::get<0>(result.nodes[2].support_reaction).value() == Catch::Approx(0.0).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[2].support_reaction).value() == Catch::Approx(20'000).epsilon(kDivergence));
      REQUIRE(std::get<0>(result.nodes[3].support_reaction).value() == Catch::Approx(-10'000).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[3].support_reaction).value() == Catch::Approx(-10'000).epsilon(kDivergence));

      // unit: millimeter [mm]
      REQUIRE(std::get<0>(result.nodes[0].displacement).value() == Catch::Approx(0.86).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[0].displacement).value() == Catch::Approx(0.18).epsilon(kDivergence));
      REQUIRE(std::get<0>(result.nodes[1].displacement).value() == Catch::Approx(1.04).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[1].displacement).value() == Catch::Approx(-0.53).epsilon(kDivergence));
      REQUIRE(std::get<0>(result.nodes[2].displacement).value() == Catch::Approx(0.18).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[2].displacement).value() == Catch::Approx(0.0).epsilon(kDivergence));
      REQUIRE(std::get<0>(result.nodes[3].displacement).value() == Catch::Approx(0.0).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[3].displacement).value() == Catch::Approx(0.0).epsilon(kDivergence));

      std::vector<double> flat_member_result{};
      std::transform(std::begin(result.members), std::end(result.members), std::back_inserter(flat_member_result),
                     [](auto&& e) { return e.normal_force.value(); });

      // unit: newton [N])
      REQUIRE(flat_member_result[0] == Catch::Approx(5'000));
      REQUIRE(flat_member_result[1] == Catch::Approx(-15'000));
      REQUIRE(flat_member_result[2] == Catch::Approx(5'000));
      REQUIRE(flat_member_result[3] == Catch::Approx(5'000));
      REQUIRE(flat_member_result[4] == Catch::Approx(7'071.0678118655));
      REQUIRE(flat_member_result[5] == Catch::Approx(-7'071.0678118655));
   }
}