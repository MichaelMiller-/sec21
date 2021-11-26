#include "approx_equal.h"
#include <catch.hpp>

#include <sec21/file_loader.h>
#include <sec21/numeric/flatten.h>
#include <sec21/numeric/ublas_allocator_wrapper.h>
#include <sec21/structural_analysis/loadcase.h>
#include <sec21/structural_analysis/solve.h>
#include <sec21/structural_analysis/node.h>
#include <sec21/structural_analysis/member.h>
#include <sec21/structural_analysis/space_truss.h>
#include <sec21/structural_analysis/solver/backend/eigen.h>

#include <array>

TEST_CASE("example system 6.0 load from json", "[sec21][structural_analysis][space_truss]")
{
   constexpr auto kDivergence{0.02};

   using namespace sec21::structural_analysis;
   using namespace sec21::units::literals;

   using precision_t= float;
   using member_t = member<int, precision_t>;
   using node_t = node<2, int, precision_t>;
   using space_truss_t = space_truss<node_t, member_t>;

   auto sys = sec21::read_from_json<space_truss_t>("example_6.json");

   REQUIRE(size(sys.nodes) == 7);
   REQUIRE(size(sys.members) == 11);
   REQUIRE(size(sys.coincidence_table) == 11);

   SECTION("test geometry")
   {
      // REQUIRE(impl::length(sys, 1) == 5.0_m);
      // REQUIRE(impl::length(sys, m2.value()) == 3.0_m);
      REQUIRE(impl::length(sys, 4) == 2.0_m);
      // REQUIRE(impl::length(sys, 5) == 2.0_m);
      REQUIRE(impl::length(sys, 6) == 4.0_m);
      REQUIRE(impl::length(sys, 7) == 4.0_m);
      // REQUIRE(impl::length(sys, m5.value()).value() == Approx(4.24264)); //_m);
      // REQUIRE(impl::length(sys, m6.value()).value() == Approx(4.24264)); //_m);

      //! \todo replace with std::numeric
      namespace bmc = boost::math::constants;
      // const auto fourth_pi{bmc::half_pi<precision_t>() * 0.5};

      REQUIRE(impl::angle_to_x_axis(sys, 1) == Approx(0.0));
      REQUIRE(impl::angle_to_x_axis(sys, 2) == Approx(0.0));
      REQUIRE(impl::angle_to_x_axis(sys, 3) == Approx(0.588));
      REQUIRE(impl::angle_to_x_axis(sys, 4) == Approx(bmc::half_pi<precision_t>()));
      REQUIRE(impl::angle_to_x_axis(sys, 5) == Approx(0.588));
      REQUIRE(impl::angle_to_x_axis(sys, 6) == Approx(bmc::half_pi<precision_t>()));
      REQUIRE(impl::angle_to_x_axis(sys, 7) == Approx(bmc::half_pi<precision_t>()));
      REQUIRE(impl::angle_to_x_axis(sys, 8) == Approx(-0.588));
      REQUIRE(impl::angle_to_x_axis(sys, 9) == Approx(bmc::half_pi<precision_t>()));
      REQUIRE(impl::angle_to_x_axis(sys, 10) == Approx(0.0));
      REQUIRE(impl::angle_to_x_axis(sys, 11) == Approx(0.0));
   }
#if 0   
   SECTION("Steifigkeitsbeziehung vom Fachwerkstab 3 in globalen Koordinaten")
   {
      auto result = impl::steifigkeitsbeziehung_fachwerkstab_globalen_koordinaten(sys, 3);
      REQUIRE(result.size1() == 4);
      REQUIRE(result.size2() == 4);
      // clang-format off
      const auto expected = std::array{
          0.49923, 0.33282,-0.49923,-0.33282,
          0.33282, 0.22188,-0.33282,-0.22188,
         -0.49923,-0.33282, 0.49923, 0.33282,     
         -0.33282,-0.22188, 0.33282, 0.22188
      };
      // clang-format on
      REQUIRE(approx_equal(sec21::numeric::flatten(result), expected, kDivergence));
   }   
   SECTION("Steifigkeitsbeziehung vom Fachwerkstab 5 in globalen Koordinaten")
   {
      auto result = impl::steifigkeitsbeziehung_fachwerkstab_globalen_koordinaten(sys, 5);
      REQUIRE(result.size1() == 4);
      REQUIRE(result.size2() == 4);
      // clang-format off
      const auto expected = std::array{
          0.49923, 0.33282,-0.49923,-0.33282,
          0.33282, 0.22188,-0.33282,-0.22188,
         -0.49923,-0.33282, 0.49923, 0.33282,     
         -0.33282,-0.22188, 0.33282, 0.22188
      };
      // clang-format on
      REQUIRE(approx_equal(sec21::numeric::flatten(result), expected, kDivergence));
   }
#endif
#if 0   
   SECTION("stiffness matrix from member 1")
   {
      auto K = impl::stiffness_matrix(sys, 1);
      REQUIRE(K.size1() == 14);
      REQUIRE(K.size2() == 14);
      // clang-format off
      const auto expected = std::array{
         1,0,-1,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         -1,0,1,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0
      };
      // clang-format on
      const auto flattend_K = sec21::numeric::flatten(K);
      REQUIRE(std::size(flattend_K) == std::size(expected));
      REQUIRE(approx_equal(flattend_K, expected, kDivergence));
   }
   SECTION("stiffness matrix from member 2")
   {
      auto K = impl::stiffness_matrix(sys, 2);
      REQUIRE(K.size1() == 14);
      REQUIRE(K.size2() == 14);
      // clang-format off
      const auto expected = std::array{
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,1,0,-1,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,-1,0,1,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0
      };
      // clang-format on
      const auto flattend_K = sec21::numeric::flatten(K);
      REQUIRE(std::size(flattend_K) == std::size(expected));
      REQUIRE(approx_equal(flattend_K, expected, kDivergence));
   }
   SECTION("stiffness matrix from member 3")
   {
      auto K = impl::stiffness_matrix(sys, 3);
      REQUIRE(K.size1() == 14);
      REQUIRE(K.size2() == 14);

// siffness matrix K:[14,14](
//    (0.257143,0.342857,0,0,0,0,-0.257143,-0.342857,0,0,0,0,0,0),
//    (0.342857,0.457143,0,0,0,0,-0.342857,-0.457143,0,0,0,0,0,0),
//    (0,0,0,0,0,0,0,0,0,0,0,0,0,0),
//    (0,0,0,0,0,0,0,0,0,0,0,0,0,0),
//    (0,0,0,0,0,0,0,0,0,0,0,0,0,0),
//    (0,0,0,0,0,0,0,0,0,0,0,0,0,0),
//    (-0.257143,-0.342857,0,0,0,0,0.257143,0.342857,0,0,0,0,0,0),
//    (-0.342857,-0.457143,0,0,0,0,0.342857,0.457143,0,0,0,0,0,0),
//    (0,0,0,0,0,0,0,0,0,0,0,0,0,0),
//    (0,0,0,0,0,0,0,0,0,0,0,0,0,0),
//    (0,0,0,0,0,0,0,0,0,0,0,0,0,0),
//    (0,0,0,0,0,0,0,0,0,0,0,0,0,0),
//    (0,0,0,0,0,0,0,0,0,0,0,0,0,0),
//    (0,0,0,0,0,0,0,0,0,0,0,0,0,0))

      std::cout << "siffness matrix K:" << K << std::endl;
      // clang-format off
      const auto expected = std::array{
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0
      };
      // clang-format on
      const auto flattend_K = sec21::numeric::flatten(K);
      REQUIRE(std::size(flattend_K) == std::size(expected));
      REQUIRE(approx_equal(flattend_K, expected, kDivergence));
   }      
   SECTION("stiffness matrix from member 4")
   {
      auto K = impl::stiffness_matrix(sys, 4);
      REQUIRE(K.size1() == 14);
      REQUIRE(K.size2() == 14);
      std::cout << "siffness matrix K:" << K << std::endl;
      // clang-format off
      const auto expected = std::array{
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,1,0,0,0,-1,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,-1,0,0,0,1,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0
      };
      // clang-format on
      const auto flattend_K = sec21::numeric::flatten(K);
      REQUIRE(std::size(flattend_K) == std::size(expected));
      REQUIRE(approx_equal(flattend_K, expected, kDivergence));
   }
   SECTION("stiffness matrix from member 5")
   {
      auto K = impl::stiffness_matrix(sys, 5);
      REQUIRE(K.size1() == 14);
      REQUIRE(K.size2() == 14);

      std::cout << "siffness matrix K:" << K << std::endl;
      // clang-format off
      const auto expected = std::array{
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0
      };
      // clang-format on
      const auto flattend_K = sec21::numeric::flatten(K);
      REQUIRE(std::size(flattend_K) == std::size(expected));
      REQUIRE(approx_equal(flattend_K, expected, kDivergence));
   }      
   SECTION("stiffness matrix from member 6")
   {
      auto K = impl::stiffness_matrix(sys, 6);
      REQUIRE(K.size1() == 14);
      REQUIRE(K.size2() == 14);
      // clang-format off
      const auto expected = std::array{
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,1,0,0,0,0,0,0,0,-1,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,-1,0,0,0,0,0,0,0,1
      };
      // clang-format on
      const auto flattend_K = sec21::numeric::flatten(K);
      REQUIRE(std::size(flattend_K) == std::size(expected));
      REQUIRE(approx_equal(flattend_K, expected, kDivergence));
   }      
   SECTION("stiffness matrix from member 7")
   {
      auto K = impl::stiffness_matrix(sys, 7);
      REQUIRE(K.size1() == 14);
      REQUIRE(K.size2() == 14);
      // clang-format off
      const auto expected = std::array{
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,1,0,0,0,0,0,0,0,-1,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,-1,0,0,0,0,0,0,0,1,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0
      };
      // clang-format on
      const auto flattend_K = sec21::numeric::flatten(K);
      REQUIRE(std::size(flattend_K) == std::size(expected));
      REQUIRE(approx_equal(flattend_K, expected, kDivergence));
   }      
   SECTION("stiffness matrix from member 8")
   {
      auto K = impl::stiffness_matrix(sys, 8);
      REQUIRE(K.size1() == 14);
      REQUIRE(K.size2() == 14);

      std::cout << "siffness matrix K:" << K << std::endl;
      // clang-format off
      const auto expected = std::array{
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0
      };
      // clang-format on
      const auto flattend_K = sec21::numeric::flatten(K);
      REQUIRE(std::size(flattend_K) == std::size(expected));
      REQUIRE(approx_equal(flattend_K, expected, kDivergence));
   }      
   SECTION("stiffness matrix from member 9")
   {
      auto K = impl::stiffness_matrix(sys, 9);
      REQUIRE(K.size1() == 14);
      REQUIRE(K.size2() == 14);
      // clang-format off
      const auto expected = std::array{
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,1,0,0,0,-1,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,-1,0,0,0,1,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0
      };
      // clang-format on
      const auto flattend_K = sec21::numeric::flatten(K);
      REQUIRE(std::size(flattend_K) == std::size(expected));
      REQUIRE(approx_equal(flattend_K, expected, kDivergence));
   }      
   SECTION("stiffness matrix from member 10")
   {
      auto K = impl::stiffness_matrix(sys, 10);
      REQUIRE(K.size1() == 14);
      REQUIRE(K.size2() == 14);

      std::cout << "siffness matrix K:" << K << std::endl;
      // clang-format off
      const auto expected = std::array{
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,1,0,-1,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,-1,0,1,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0
      };
      // clang-format on
      const auto flattend_K = sec21::numeric::flatten(K);
      REQUIRE(std::size(flattend_K) == std::size(expected));
      REQUIRE(approx_equal(flattend_K, expected, kDivergence));
   }      
   SECTION("stiffness matrix from member 11")
   {
      auto K = impl::stiffness_matrix(sys, 11);
      REQUIRE(K.size1() == 14);
      REQUIRE(K.size2() == 14);
      // clang-format off
      const auto expected = std::array{
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,-1,0,1,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,-1,0,1,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0
      };
      // clang-format on
      const auto flattend_K = sec21::numeric::flatten(K);
      REQUIRE(std::size(flattend_K) == std::size(expected));
      REQUIRE(approx_equal(flattend_K, expected, kDivergence));
   }
#endif
   SECTION("coincidence matrix from member 1")
   {
      using allocator_t = sec21::numeric::ublas_allocator_wrapper<std::allocator<precision_t>>;
      auto Z = impl::coincidence_matrix<allocator_t>(sys, 1);
      REQUIRE(Z.size1() == 4);
      REQUIRE(Z.size2() == 14);
      // clang-format off
      const auto expected = std::array<precision_t, 4*14>{
         1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
         0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
         0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
         0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      };
      // clang-format on
      REQUIRE(approx_equal(sec21::numeric::flatten(Z), expected, kDivergence));
   }
#if 0
   SECTION("coincidence matrix from member 6")
   {
      auto Z = impl::coincidence_matrix(sys, 6);
      REQUIRE(Z.size1() == 4);
      REQUIRE(Z.size2() == 14);
      // clang-format off
      const auto expected = std::array{
         0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
         0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1
      };
      // clang-format on
      REQUIRE(approx_equal(sec21::numeric::flatten(Z), expected, kDivergence)); 
   }
#endif
   SECTION("solve")
   {
      auto load = sec21::read_from_json<loadcase<decltype(sys)>>("example_6_load.json");

      const auto success = solve<solver::backend::eigen>(sys, load);
      REQUIRE(success.has_value() == true);

      const auto result = success.value();

      // unit: newton [N]
      REQUIRE(std::get<0>(result.nodes[0].support_reaction).value() == Approx(0.0).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[0].support_reaction).value() == Approx(0.0).epsilon(kDivergence));
      REQUIRE(std::get<0>(result.nodes[1].support_reaction).value() == Approx(30'000).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[1].support_reaction).value() == Approx(95'000).epsilon(kDivergence));
      REQUIRE(std::get<0>(result.nodes[2].support_reaction).value() == Approx(0.0).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[2].support_reaction).value() == Approx(-45'000).epsilon(kDivergence));

      // unit: millimeter [mm]
      REQUIRE(std::get<0>(result.nodes[6].displacement).value() == Approx(-13.7).epsilon(kDivergence));

      std::vector<double> copied_results{};
      std::transform(std::begin(result.members), std::end(result.members), std::back_inserter(copied_results),
                     [](auto&& m) { return m.normal_force.value(); });

      // unit: newton [N]
      REQUIRE(copied_results[0] == Approx(-37'500));
      REQUIRE(copied_results[1] == Approx(0.0));
      REQUIRE(copied_results[2] == Approx(-45'070).epsilon(kDivergence));
      REQUIRE(copied_results[3] == Approx(-50'000));
      REQUIRE(copied_results[4] == Approx(-81'120).epsilon(kDivergence));
      REQUIRE(copied_results[5] == Approx(45'000));
      REQUIRE(copied_results[6] == Approx(-25'000));
      REQUIRE(copied_results[7] == Approx(-45'070).epsilon(kDivergence));
      // REQUIRE(copied_results[8] == Approx(0.0));
      REQUIRE(copied_results[9] == Approx(67'500));
      REQUIRE(copied_results[10] == Approx(67'500));
   }
}