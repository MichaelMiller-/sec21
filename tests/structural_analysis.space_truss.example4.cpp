#include <catch.hpp>
#include "approx_equal.h"

#include <sec21/file_loader.h>
#include <sec21/structural_analysis/node.h>
#include <sec21/structural_analysis/member.h>
#include <sec21/structural_analysis/space_truss.h>
#include <sec21/structural_analysis/loadcase.h>
#include <sec21/structural_analysis/system_result.h>
#include <sec21/structural_analysis/solve.h>
#include <sec21/structural_analysis/solver/backend/viennacl.h>

#include <array>

TEST_CASE("example system 4.0 load from json", "[sec21][structural_analysis][space_truss]")
{
   constexpr auto kDivergence{0.01};

   using namespace sec21::structural_analysis;
   using namespace sec21::units::literals;

   using member_t = member<int, double>;
   using node_t = node<2, int, double>;
   using space_truss_t = space_truss<node_t, member_t>;

   auto sys = sec21::load_from_json<space_truss_t>("example_4.json");
   auto lf1 = sec21::load_from_json<loadcase<decltype(sys)>>("example_4_load.json");

   auto [success, result] = solve<solver::backend::viennacl_impl>(sys, lf1);
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
      std::begin(result.members), 
      std::end(result.members), 
      std::back_inserter(copied_results), 
      [](auto&& m) { return m.normal_force.value(); });

   // unit: newton [N]
   REQUIRE(flat_support_reaction[0] == Approx(60'000.0));
   REQUIRE(flat_support_reaction[1] == Approx(70'000.0));
   REQUIRE(flat_support_reaction[2] == Approx(-60'000.0));
   REQUIRE(flat_support_reaction[3] == Approx(70'000.0));        

   // unit: millimeter [mm]
   REQUIRE(flat_displacement[13] == Approx(-29.2).epsilon(kDivergence));

   // unit: newton [N]
   REQUIRE(copied_results[0] == Approx(30'000));
   REQUIRE(copied_results[1] == Approx(-116'619).epsilon(kDivergence));
   REQUIRE(copied_results[2] == Approx(-116'619).epsilon(kDivergence));
   REQUIRE(copied_results[3] == Approx(30'000));
   REQUIRE(copied_results[4] == Approx(40'000));
   REQUIRE(copied_results[5] == Approx(-40'000));
   REQUIRE(copied_results[6] == Approx(80'800).epsilon(kDivergence));
   REQUIRE(copied_results[7] == Approx(-141'400).epsilon(kDivergence));
   REQUIRE(copied_results[8] == Approx(141'400).epsilon(kDivergence));
   REQUIRE(copied_results[9] == Approx(80'800).epsilon(kDivergence));

   {
      std::ofstream ofs{"output_example_4_result.json"};
      nlohmann::json tmp = result;
      ofs << std::setw(4) << tmp;
   }         
}