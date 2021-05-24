#include <catch.hpp>
#include "approx_equal.h"

#include <sec21/file_loader.h>
#include <sec21/structural_analysis/loadcase.h>
#include <sec21/structural_analysis/solve.h>
#include <sec21/structural_analysis/node.h>
#include <sec21/structural_analysis/member.h>
#include <sec21/structural_analysis/space_truss.h>
#include <sec21/structural_analysis/system_result.h>
#include <sec21/structural_analysis/solver/backend/viennacl.h>

#include <array>

TEST_CASE("example system 2.0 with temperature load", "[sec21][structural_analysis][space_truss]")
{
   constexpr auto kDivergence{0.01};

   using namespace sec21::structural_analysis;
   using namespace sec21::units::literals;

   using member_t = member<int, double>;
   using node_t = node<2, int, double>;
   using space_truss_t = space_truss<node_t, member_t>;

   auto sys = sec21::load_from_json<space_truss_t>("example_2.json");
   auto load = sec21::load_from_json<loadcase<decltype(sys)>>("example_2_temperature_load.json");

   auto [success, result] = solve<solver::backend::viennacl_impl>(sys, load);
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
      std::begin(result.members), 
      std::end(result.members), 
      std::back_inserter(copied_results), 
      [](auto&& m) { return m.normal_force.value(); });

   // unit: newton [N]
   // REQUIRE(flat_support_reaction[0] == Approx(3'000.0));
   // REQUIRE(flat_support_reaction[1] == Approx(3'000.0));
   // REQUIRE(flat_support_reaction[6] == Approx(-3'000.0));
   // REQUIRE(flat_support_reaction[7] == Approx(0.0));

   // unit: millimeter [mm]
   REQUIRE(flat_displacement[3] == Approx(-7.8).epsilon(kDivergence));

   // unit: newton [N]
   // REQUIRE(copied_results[0] == Approx(1'500));
   // REQUIRE(copied_results[1] == Approx(-5'410).epsilon(kDivergence));
   // REQUIRE(copied_results[2] == Approx(-2'120).epsilon(kDivergence));
   // REQUIRE(copied_results[3] == Approx(4'500));
   // REQUIRE(copied_results[4] == Approx(-5'410).epsilon(kDivergence));

   {
      std::ofstream ofs{"output_example_2_result.json"};
      nlohmann::json tmp = result;
      ofs << std::setw(4) << tmp;
   }
}