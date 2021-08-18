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

   const auto success = solve<solver::backend::viennacl_impl>(sys, lf1);
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

   {
      std::vector<double> copied_results{};
      std::transform(
         std::begin(result.members), 
         std::end(result.members), 
         std::back_inserter(copied_results), 
         [](auto&& m) { return m.normal_force.value(); });

      // unit: newton [N]
      const auto expected = std::array{ 5'040.0, -15'120.0, 5'040.0, 5'040.0, 7'127.63635, -7'127.63635 };
      REQUIRE(approx_equal(copied_results, expected, kDivergence));
   }
}