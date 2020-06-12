﻿#include <catch.hpp>
#include "approx_equal.h"

#include <sec21/file_loader.h>
#include <sec21/flat_matrix.h>
#include <sec21/structural_analysis/space_truss.h>
#include <sec21/structural_analysis/loadcase.h>
#include <sec21/structural_analysis/system_result.h>
#include <sec21/structural_analysis/solve.h>

#include <array>
#include <valarray>

TEST_CASE("example system 2.0 load from json", "[sec21][structural_analysis][space_truss]")
{
   constexpr auto kDivergence{0.01};

   using namespace sec21::structural_analysis;
   using namespace sec21::units::literals;

   auto sys = sec21::load_from_json<space_truss>("example_2.json");
   auto load = sec21::load_from_json<loadcase<decltype(sys)>>("example_2_load.json");

   REQUIRE(std::size(sys.nodes) == 4);
   REQUIRE(std::size(sys.members) == 5);

   SECTION("coincidence matrix from member 1")
   {
      auto Z = impl::coincidence_matrix(sys, 1);
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
      auto Z = impl::coincidence_matrix(sys, 2);
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
   SECTION("coincidence matrix from member 3")
   {
      auto Z = impl::coincidence_matrix(sys, 3);
      REQUIRE(Z.size1() == 4);
      REQUIRE(Z.size2() == 8);
      //! \clang-format off
      const auto expected = std::array{
         0, 0, 1, 0, 0, 0, 0, 0,
         0, 0, 0, 1, 0, 0, 0, 0,
         0, 0, 0, 0, 1, 0, 0, 0,
         0, 0, 0, 0, 0, 1, 0, 0,
      };
      //! \clang-format on
      REQUIRE(approx_equal(sec21::flat_matrix(Z), expected, kDivergence));
   }
   SECTION("coincidence matrix from member 4")
   {
      auto Z = impl::coincidence_matrix(sys, 4);
      REQUIRE(Z.size1() == 4);
      REQUIRE(Z.size2() == 8);
      //! \clang-format off
      const auto expected = std::array{
         0, 0, 1, 0, 0, 0, 0, 0,
         0, 0, 0, 1, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 1, 0,
         0, 0, 0, 0, 0, 0, 0, 1,
      };
      //! \clang-format on
      REQUIRE(approx_equal(sec21::flat_matrix(Z), expected, kDivergence));
   }
   SECTION("coincidence matrix from member 5")
   {
      auto Z = impl::coincidence_matrix(sys, 5);
      REQUIRE(Z.size1() == 4);
      REQUIRE(Z.size2() == 8);
      //! \clang-format off
      const auto expected = std::array{
         0, 0, 0, 0, 1, 0, 0, 0,
         0, 0, 0, 0, 0, 1, 0, 0,
         0, 0, 0, 0, 0, 0, 1, 0,
         0, 0, 0, 0, 0, 0, 0, 1,
      };
      //! \clang-format on
      REQUIRE(approx_equal(sec21::flat_matrix(Z), expected, kDivergence));
   }
   SECTION("stiffness matrix from member 1")
   {
      auto K = impl::stiffness_matrix(sys, 1);
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
      const auto EA_l = 1000.0;
      expected *= EA_l;
      //! \clang-format on
      const auto flattend_K = sec21::flat_matrix(K);
      REQUIRE(std::size(flattend_K) == std::size(expected));
      REQUIRE(approx_equal(flattend_K, expected, kDivergence));
   }
   SECTION("stiffness matrix from member 4")
   {
      auto K = impl::stiffness_matrix(sys, 4);
      REQUIRE(K.size1() == 8);
      REQUIRE(K.size2() == 8);
      //! \clang-format off
      auto expected = std::valarray{
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0,-1.0,
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0,-1.0, 0.0, 0.0, 0.0, 1.0
      };
      const auto EA_l = 1000.0;
      expected *= EA_l;
      //! \clang-format on
      const auto flattend_K = sec21::flat_matrix(K);
      REQUIRE(std::size(flattend_K) == std::size(expected));
      REQUIRE(approx_equal(flattend_K, expected, kDivergence));
   }
   SECTION("solve")
   {
      auto [success, result] = solve(sys, load);
      REQUIRE(success == true);

      std::vector<double> flat_support_reaction{};
      for (auto [k,v] : result.node) 
      {
         std::transform(
            std::begin(v.support_reaction), 
            std::end(v.support_reaction), 
            std::back_inserter(flat_support_reaction),
            [](auto&& e) { return e.value(); });
      }

      std::vector<double> flat_displacement{};
      for (auto [k,v] : result.node) 
         std::transform(
            std::begin(v.displacement), 
            std::end(v.displacement), 
            std::back_inserter(flat_displacement),
            [](auto&& e) { return e.value(); });

      std::vector<double> copied_results{};
      std::transform(
         std::begin(result.member), 
         std::end(result.member), 
         std::back_inserter(copied_results), 
         [](auto&& m) { return m.second.normal_force.value(); });

      // unit: newton [N]
      REQUIRE(flat_support_reaction[0] == Approx(3'000.0));
      REQUIRE(flat_support_reaction[1] == Approx(3'000.0));
      REQUIRE(flat_support_reaction[6] == Approx(-3'000.0));
      REQUIRE(flat_support_reaction[7] == Approx(0.0));

      // unit: millimeter [mm]
      REQUIRE(flat_displacement[2] == Approx(1.5).epsilon(kDivergence));
      REQUIRE(flat_displacement[3] == Approx(-22.4).epsilon(kDivergence));

      // unit: newton [N]
      REQUIRE(copied_results[0] == Approx(1'500));
      REQUIRE(copied_results[1] == Approx(-5'410).epsilon(kDivergence));
      REQUIRE(copied_results[2] == Approx(-2'120).epsilon(kDivergence));
      REQUIRE(copied_results[3] == Approx(4'500));
      REQUIRE(copied_results[4] == Approx(-5'410).epsilon(kDivergence));

      {
         std::ofstream ofs{"output_example_2_result.json"};
         nlohmann::json tmp = result;
         ofs << std::setw(4) << tmp;
      }
   }
}