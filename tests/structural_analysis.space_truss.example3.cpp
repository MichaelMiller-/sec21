#include <catch.hpp>

#include <sec21/file_loader.h>
#include <sec21/structural_analysis/node.h>
#include <sec21/structural_analysis/member.h>
#include <sec21/structural_analysis/space_truss.h>
#include <sec21/structural_analysis/loadcase.h>
#include <sec21/structural_analysis/solve.h>
#include <sec21/structural_analysis/solver/backend/eigen.h>

#include <array>

TEST_CASE("example system 3.0 load from json", "[sec21][structural_analysis][space_truss]")
{
   constexpr auto kDivergence{0.02};

   using namespace sec21::structural_analysis;
   using namespace sec21::units::literals;

   using member_t = member<int, double>;
   using node_t = node<2, int, double>;
   using space_truss_t = space_truss<node_t, member_t>;

   auto sys = sec21::load_from_json<space_truss_t>("example_3.json");

   REQUIRE(size(sys.nodes) == 7);
   REQUIRE(size(sys.members) == 10);
   REQUIRE(size(sys.coincidence_table) == 10);

   SECTION("test geometry")
   {
      REQUIRE(impl::length(sys, 1) == 5.0_m);
      REQUIRE(impl::length(sys, 4) == 7.0_m);
      REQUIRE(impl::length(sys, 5) == 2.0_m);
      REQUIRE(impl::length(sys, 6) == 2.0_m);
   }
   SECTION("E*A units from first member")
   {
      auto it = std::begin(sys.members);
      auto EA = it->E * it->A;
      REQUIRE(EA.value() == Approx(884100));
   }
#if 0
   SECTION("solve")
   {
      auto lf1 = sec21::load_from_json<loadcase<decltype(sys)>>("example_3_load.json");

      const auto success = solve<solver::backend::viennacl_impl>(sys, lf1);
      REQUIRE(success.has_value() == true);

      const auto result = success.value();

      // unit: newton [N]
      REQUIRE(std::get<0>(result.nodes[0].support_reaction).value() == Approx(68'000).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[0].support_reaction).value() == Approx(76'000).epsilon(kDivergence));
      REQUIRE(std::get<0>(result.nodes[1].support_reaction).value() == Approx(60'000).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[1].support_reaction).value() == Approx(-76'000).epsilon(kDivergence));

      // unit: millimeter [mm]
      REQUIRE(std::get<0>(result.nodes[0].displacement).value() == Approx(0.0).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[0].displacement).value() == Approx(0.0).epsilon(kDivergence));
      REQUIRE(std::get<0>(result.nodes[1].displacement).value() == Approx(0.0).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[1].displacement).value() == Approx(0.0).epsilon(kDivergence));
      REQUIRE(std::get<0>(result.nodes[2].displacement).value() == Approx(-21.7).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[2].displacement).value() == Approx(0.53).epsilon(kDivergence));
      REQUIRE(std::get<0>(result.nodes[3].displacement).value() == Approx(-22.1).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[3].displacement).value() == Approx(7.65).epsilon(kDivergence));
      REQUIRE(std::get<0>(result.nodes[4].displacement).value() == Approx(-27.1).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[4].displacement).value() == Approx(-5.88).epsilon(kDivergence));
      REQUIRE(std::get<0>(result.nodes[5].displacement).value() == Approx(-26.5).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[5].displacement).value() == Approx(-1.06).epsilon(kDivergence));
      REQUIRE(std::get<0>(result.nodes[6].displacement).value() == Approx(-21.8).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[6].displacement).value() == Approx(4.89).epsilon(kDivergence));

      // unit: newton [N]
      REQUIRE(result.members[0].normal_force.value() == Approx(94'000));
      REQUIRE(result.members[1].normal_force.value() == Approx(-183'095).epsilon(kDivergence));
      REQUIRE(result.members[2].normal_force.value() == Approx(218'400).epsilon(kDivergence));
      REQUIRE(result.members[3].normal_force.value() == Approx(-134'000));
      REQUIRE(result.members[4].normal_force.value() == Approx(187'000));
      REQUIRE(result.members[5].normal_force.value() == Approx(255'000));
      REQUIRE(result.members[6].normal_force.value() == Approx(253'100).epsilon(kDivergence));
      REQUIRE(result.members[7].normal_force.value() == Approx(-306'400).epsilon(kDivergence));
      REQUIRE(result.members[8].normal_force.value() == Approx(-378'600).epsilon(kDivergence));
      REQUIRE(result.members[9].normal_force.value() == Approx(-360'800).epsilon(kDivergence));
   }
#endif
   SECTION("solv with eigen backend")
   {
      auto lf1 = sec21::load_from_json<loadcase<decltype(sys)>>("example_3_load.json");

      const auto success = solve<solver::backend::eigen>(sys, lf1);
      REQUIRE(success.has_value() == true);

      const auto result = success.value();

      // unit: newton [N]
      REQUIRE(std::get<0>(result.nodes[0].support_reaction).value() == Approx(68'000).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[0].support_reaction).value() == Approx(76'000).epsilon(kDivergence));
      REQUIRE(std::get<0>(result.nodes[1].support_reaction).value() == Approx(60'000).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[1].support_reaction).value() == Approx(-76'000).epsilon(kDivergence));

      // unit: millimeter [mm]
      REQUIRE(std::get<0>(result.nodes[0].displacement).value() == Approx(0.0).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[0].displacement).value() == Approx(0.0).epsilon(kDivergence));
      REQUIRE(std::get<0>(result.nodes[1].displacement).value() == Approx(0.0).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[1].displacement).value() == Approx(0.0).epsilon(kDivergence));
      REQUIRE(std::get<0>(result.nodes[2].displacement).value() == Approx(-21.7).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[2].displacement).value() == Approx(0.53).epsilon(kDivergence));
      REQUIRE(std::get<0>(result.nodes[3].displacement).value() == Approx(-22.1).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[3].displacement).value() == Approx(7.65).epsilon(kDivergence));
      REQUIRE(std::get<0>(result.nodes[4].displacement).value() == Approx(-27.1).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[4].displacement).value() == Approx(-5.88).epsilon(kDivergence));
      REQUIRE(std::get<0>(result.nodes[5].displacement).value() == Approx(-26.5).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[5].displacement).value() == Approx(-1.06).epsilon(kDivergence));
      REQUIRE(std::get<0>(result.nodes[6].displacement).value() == Approx(-21.8).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[6].displacement).value() == Approx(4.89).epsilon(kDivergence));

      // unit: newton [N]
      REQUIRE(result.members[0].normal_force.value() == Approx(94'000));
      REQUIRE(result.members[1].normal_force.value() == Approx(-183'095).epsilon(kDivergence));
      REQUIRE(result.members[2].normal_force.value() == Approx(218'400).epsilon(kDivergence));
      REQUIRE(result.members[3].normal_force.value() == Approx(-134'000));
      REQUIRE(result.members[4].normal_force.value() == Approx(187'000));
      REQUIRE(result.members[5].normal_force.value() == Approx(255'000));
      REQUIRE(result.members[6].normal_force.value() == Approx(253'100).epsilon(kDivergence));
      REQUIRE(result.members[7].normal_force.value() == Approx(-306'400).epsilon(kDivergence));
      REQUIRE(result.members[8].normal_force.value() == Approx(-378'600).epsilon(kDivergence));
      REQUIRE(result.members[9].normal_force.value() == Approx(-360'800).epsilon(kDivergence));
   }
}