#include <catch.hpp>

#include <sec21/file_loader.h>
#include <sec21/structural_analysis/node.h>
#include <sec21/structural_analysis/member.h>
#include <sec21/structural_analysis/space_truss.h>
#include <sec21/structural_analysis/loadcase.h>
#include <sec21/structural_analysis/solve.h>
#include <sec21/structural_analysis/solver/backend/eigen.h>

#include <array>

TEST_CASE("example system 4.0 load from json", "[sec21][structural_analysis][space_truss]")
{
   constexpr auto kDivergence{0.01};

   using namespace sec21::structural_analysis;
   using namespace sec21::units::literals;

   using precision_t = double;
   using member_t = member<int, precision_t>;
   using node_t = node<2, int, precision_t>;
   using space_truss_t = space_truss<node_t, member_t>;

   auto sys = sec21::read_from_json<space_truss_t>("example_4.json");

   REQUIRE(size(sys.nodes) == 7);
   REQUIRE(size(sys.members) == 10);
   REQUIRE(size(sys.coincidence_table) == 10);

   auto lf1 = sec21::read_from_json<loadcase<decltype(sys)>>("example_4_load.json");

   const auto success = solve<solver::backend::eigen>(sys, lf1);
   REQUIRE(success.has_value() == true);

   const auto result = success.value();

   std::vector<precision_t> flat_support_reaction{};
   for (auto [k,v] : result.node) 
   {
      std::transform(
         std::begin(v.support_reaction), 
         std::end(v.support_reaction), 
         std::back_inserter(flat_support_reaction),
         [](auto&& e) { return e.value(); });
   }

   std::vector<precision_t> copied_results{};
   std::transform(
      std::begin(result.members), 
      std::end(result.members), 
      std::back_inserter(copied_results), 
      [](auto&& m) { return m.normal_force.value(); });

   // unit: newton [N]
   REQUIRE(std::get<0>(result.nodes[0].support_reaction).value() == Catch::Approx(60'000).epsilon(kDivergence));
   REQUIRE(std::get<1>(result.nodes[0].support_reaction).value() == Catch::Approx(70'000).epsilon(kDivergence));
   REQUIRE(std::get<0>(result.nodes[1].support_reaction).value() == Catch::Approx(-60'000).epsilon(kDivergence));
   REQUIRE(std::get<1>(result.nodes[1].support_reaction).value() == Catch::Approx(70'000).epsilon(kDivergence));

   // unit: millimeter [mm]
   REQUIRE(std::get<1>(result.nodes[6].displacement).value() == Catch::Approx(-29.2).epsilon(kDivergence));

   // unit: newton [N]
   REQUIRE(copied_results[0] == Catch::Approx(30'000));
   REQUIRE(copied_results[1] == Catch::Approx(-116'619).epsilon(kDivergence));
   REQUIRE(copied_results[2] == Catch::Approx(-116'619).epsilon(kDivergence));
   REQUIRE(copied_results[3] == Catch::Approx(30'000));
   REQUIRE(copied_results[4] == Catch::Approx(40'000));
   REQUIRE(copied_results[5] == Catch::Approx(-40'000));
   REQUIRE(copied_results[6] == Catch::Approx(80'800).epsilon(kDivergence));
   REQUIRE(copied_results[7] == Catch::Approx(-141'400).epsilon(kDivergence));
   REQUIRE(copied_results[8] == Catch::Approx(141'400).epsilon(kDivergence));
   REQUIRE(copied_results[9] == Catch::Approx(80'800).epsilon(kDivergence));
}