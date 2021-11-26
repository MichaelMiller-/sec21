#include <catch.hpp>

#include <sec21/file_loader.h>
#include <sec21/structural_analysis/loadcase.h>
#include <sec21/structural_analysis/member.h>
#include <sec21/structural_analysis/node.h>
#include <sec21/structural_analysis/solve.h>
#include <sec21/structural_analysis/solver/backend/eigen.h>
#include <sec21/structural_analysis/space_truss.h>

#include <array>

TEST_CASE("example system 7.0 load from json", "[sec21][structural_analysis][space_truss]")
{
   constexpr auto kDivergence{0.01};

   using namespace sec21::structural_analysis;
   using namespace sec21::units::literals;

   using precision_t = double;
   using member_t = member<int, precision_t>;
   using node_t = node<2, int, precision_t>;
   using space_truss_t = space_truss<node_t, member_t>;

   auto sys = sec21::read_from_json<space_truss_t>("example_7.json");
   auto lf1 = sec21::read_from_json<loadcase<decltype(sys)>>("example_7_load.json");

   const auto success = solve<solver::backend::eigen>(sys, lf1);
   REQUIRE(success.has_value() == true);

   const auto result = success.value();

   REQUIRE(std::get<0>(result.nodes[3].support_reaction).value() == Approx(-30'000).epsilon(kDivergence));
   REQUIRE(std::get<1>(result.nodes[3].support_reaction).value() == Approx(102'000).epsilon(kDivergence));
   REQUIRE(std::get<0>(result.nodes[6].support_reaction).value() == Approx(0.0).epsilon(kDivergence));
   REQUIRE(std::get<1>(result.nodes[6].support_reaction).value() == Approx(98'000).epsilon(kDivergence));

   // unit: millimeter [mm]
   REQUIRE(std::get<1>(result.nodes[0].displacement).value() == Approx(-13.9).epsilon(kDivergence));

   std::vector<precision_t> copied_results{};
   std::transform(std::begin(result.members), std::end(result.members), std::back_inserter(copied_results),
                  [](auto&& m) { return m.normal_force.value(); });

   // unit: newton [N]
   REQUIRE(copied_results[0] == Approx(75000));
   REQUIRE(copied_results[1] == Approx(25000));
   REQUIRE(copied_results[2] == Approx(-15000));
   REQUIRE(copied_results[3] == Approx(80000));
   REQUIRE(copied_results[4] == Approx(-36055).epsilon(kDivergence));
   REQUIRE(copied_results[5] == Approx(20000).epsilon(kDivergence));
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
}