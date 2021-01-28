#include <catch.hpp>
#include "approx_equal.h"

#include <sec21/file_loader.h>
#include <sec21/structural_analysis/node.h>
#include <sec21/structural_analysis/member.h>
#include <sec21/structural_analysis/space_truss.h>
#include <sec21/structural_analysis/loadcase.h>
#include <sec21/structural_analysis/system_result.h>
#include <sec21/structural_analysis/solve.h>

TEST_CASE("example system 1.0 load from json", "[sec21][structural_analysis][space_truss]")
{
   constexpr auto kDivergence{0.02};

   using namespace sec21::structural_analysis;
   using namespace sec21::units::literals;

   using member_t = member<int, double>;
   using node_t = node<2, int, double>;
   using space_truss_t = space_truss<node_t, member_t>;

   auto sys = sec21::load_from_json<space_truss_t>("example_1.json");

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
      const auto expected = std::array{ 0.0869117, 0.018, 0.1049117, -0.054, 0.018, 0.0, 0.0, 0.0 };
      REQUIRE(approx_equal(flat_displacement, expected, kDivergence));
   }
   {
      std::vector<double> copied_results{};
      std::transform(
         std::begin(result.member), 
         std::end(result.member), 
         std::back_inserter(copied_results), 
         [](auto&& m) { return m.second.normal_force.value(); });

      // unit: newton [N]
      const auto expected = std::array{ 5'040.0, -15'120.0, 5'040.0, 5'040.0, 7'127.63635, -7'127.63635 };
      REQUIRE(approx_equal(copied_results, expected, kDivergence));
   }
}