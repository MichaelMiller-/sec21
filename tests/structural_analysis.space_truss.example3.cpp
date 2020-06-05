#include <catch.hpp>
#include "approx_equal.h"

#include <sec21/file_loader.h>
#include <sec21/structural_analysis/space_truss.h>
#include <sec21/structural_analysis/loadcase.h>
#include <sec21/structural_analysis/system_result.h>
#include <sec21/structural_analysis/solve.h>

#include <array>

TEST_CASE("example system 3.0 load from json", "[sec21][structural_analysis][space_truss]")
{
   constexpr auto kDivergence{0.02};

   using namespace sec21::structural_analysis;
   using namespace sec21::units::literals;

   auto sys = sec21::load_from_json<space_truss>("example_3.json");

   SECTION("test geometry")
   {
      REQUIRE(impl::length(sys, 1) == 5.0_m);
      REQUIRE(impl::length(sys, 4) == 7.0_m);
      REQUIRE(impl::length(sys, 5) == 2.0_m);
      REQUIRE(impl::length(sys, 6) == 2.0_m);
   }
   SECTION("solve")
   {
      auto load = sec21::load_from_json<loadcase<decltype(sys)>>("example_3_load.json");

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
      {
         std::transform(
            std::begin(v.displacement), 
            std::end(v.displacement), 
            std::back_inserter(flat_displacement),
            [](auto&& e) { return e.value(); });
      }

      std::vector<double> copied_results{};
      std::transform(
         std::begin(result.member), 
         std::end(result.member), 
         std::back_inserter(copied_results), 
         [](auto&& m) { return m.second.normal_force.value(); });

      // unit: newton [N]
      REQUIRE(flat_support_reaction[0] == Approx(68'000.0));
      REQUIRE(flat_support_reaction[1] == Approx(76'000.0));
      REQUIRE(flat_support_reaction[2] == Approx(60'000.0));
      REQUIRE(flat_support_reaction[3] == Approx(-76'000.0));        

      // // unit: millimeter [mm]
      REQUIRE(flat_displacement[10] == Approx(-26.7).epsilon(kDivergence));

      // // unit: newton [N]
      REQUIRE(copied_results[0] == Approx(94'000));
      REQUIRE(copied_results[1] == Approx(-183'095).epsilon(kDivergence));
      REQUIRE(copied_results[2] == Approx(218'400).epsilon(kDivergence));
      REQUIRE(copied_results[3] == Approx(-134'000));
      REQUIRE(copied_results[4] == Approx(187'000));
      REQUIRE(copied_results[5] == Approx(255'000));
      REQUIRE(copied_results[6] == Approx(253'100).epsilon(kDivergence));
      REQUIRE(copied_results[7] == Approx(-306'400).epsilon(kDivergence));
      REQUIRE(copied_results[8] == Approx(-378'600).epsilon(kDivergence));
      REQUIRE(copied_results[9] == Approx(-360'800).epsilon(kDivergence));

      {
         std::ofstream ofs{"output_example_3_result.json"};
         nlohmann::json tmp = result;
         ofs << std::setw(4) << tmp;
      }         
   }
}