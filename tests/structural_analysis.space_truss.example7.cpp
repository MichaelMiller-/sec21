#include <catch.hpp>
#include "approx_equal.h"

#include <sec21/file_loader.h>
#include <sec21/structural_analysis/space_truss.h>
#include <sec21/structural_analysis/loadcase.h>
#include <sec21/structural_analysis/system_result.h>
#include <sec21/structural_analysis/solve.h>

#include <array>

TEST_CASE("example system 7.0 load from json", "[sec21][structural_analysis][space_truss]")
{
   constexpr auto kDivergence{0.01};

   using namespace sec21::structural_analysis;
   using namespace sec21::units::literals;

   auto sys = sec21::load_from_json<space_truss>("example_7.json");
   auto load = sec21::load_from_json<loadcase<decltype(sys)>>("example_7_load.json");

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
   REQUIRE(flat_support_reaction[0] == Approx(0.0));
   REQUIRE(flat_support_reaction[1] == Approx(0.0));
   REQUIRE(flat_support_reaction[6] == Approx(-30'000.0));
   REQUIRE(flat_support_reaction[7] == Approx(102'000.0));
   REQUIRE(flat_support_reaction[13] == Approx(98'000.0));

   // unit: millimeter [mm]
   REQUIRE(flat_displacement[1] == Approx(-13.9).epsilon(kDivergence));

   // unit: newton [N]
   REQUIRE(copied_results[0] == Approx(75000));
   REQUIRE(copied_results[1] == Approx(25000));
   REQUIRE(copied_results[2] == Approx(-15000));
   REQUIRE(copied_results[3] == Approx(80000));
   REQUIRE(copied_results[4] == Approx(-36055).epsilon(kDivergence));
   REQUIRE(copied_results[5] == Approx(20000));
   REQUIRE(copied_results[6] == Approx(234000));
   REQUIRE(copied_results[7] == Approx(-177000));
   REQUIRE(copied_results[8] == Approx(147000));
   REQUIRE(copied_results[10] == Approx(-228079));
   REQUIRE(copied_results[11] == Approx(42000));
   REQUIRE(copied_results[12] == Approx(68505));
   REQUIRE(copied_results[14] == Approx(-176672));
   REQUIRE(copied_results[17] == Approx(204000));
   REQUIRE(copied_results[19] == Approx(60000));
   REQUIRE(copied_results[21] == Approx(-60000));
   REQUIRE(copied_results[23] == Approx(60000));
   REQUIRE(copied_results[25] == Approx(-60000));
   REQUIRE(copied_results[26] == Approx(45000));
   REQUIRE(copied_results[27] == Approx(-75000));
   REQUIRE(copied_results[28] == Approx(-75000));

   {
      std::ofstream ofs{"output_example_7_result.json"};
      nlohmann::json tmp = result;
      ofs << std::setw(4) << tmp;
   }         
}