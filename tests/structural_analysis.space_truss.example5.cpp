#include <catch.hpp>

#include <sec21/file_loader.h>
#include <sec21/structural_analysis/node.h>
#include <sec21/structural_analysis/member.h>
#include <sec21/structural_analysis/space_truss.h>
#include <sec21/structural_analysis/loadcase.h>
#include <sec21/structural_analysis/solve.h>
#include <sec21/structural_analysis/solver/backend/eigen.h>

#include <array>

TEST_CASE("example system 5.0 load from json", "[sec21][structural_analysis][space_truss]")
{
   constexpr auto kDivergence{0.01};

   using namespace sec21::structural_analysis;
   using namespace sec21::units::literals;

   using member_t = member<int, double>;
   using node_t = node<2, int, double>;
   using space_truss_t = space_truss<node_t, member_t>;

   auto sys = sec21::read_from_json<space_truss_t>("example_5.json");

   REQUIRE(std::size(sys.nodes) == 9);
   REQUIRE(size(sys.members) == 14);
   REQUIRE(size(sys.coincidence_table) == 14);

   SECTION("test geometry")
   {
      REQUIRE(impl::length(sys, 3) == 5.0_m);
      REQUIRE(impl::length(sys, 4) == 5.0_m);
      REQUIRE(impl::length(sys, 5) == 3.0_m);
      REQUIRE(impl::length(sys, 6) == 3.0_m);
      REQUIRE(impl::length(sys, 13) == 4.0_m);
   }
   SECTION("E*A quantity from first member")
   {
      auto it = std::begin(sys.members);
      auto EA = it->E * it->A;
      REQUIRE(EA.value() == Approx(200'000));
   }
#if 0
   SECTION("solve")
   {
      auto load = sec21::load_from_json<loadcase<decltype(sys)>>("example_5_load.json");

      const auto success = solve<solver::backend::viennacl_impl>(sys, load);
      REQUIRE(success.has_value() == true);

      const auto result = success.value();

      // unit: newton [N]
      REQUIRE(std::get<0>(result.nodes[0].support_reaction).value() == Approx(-12'000).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[0].support_reaction).value() == Approx(52'000).epsilon(kDivergence));
      REQUIRE(std::get<0>(result.nodes[1].support_reaction).value() == Approx(12'000).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[1].support_reaction).value() == Approx(52'000).epsilon(kDivergence));

      // // unit: millimeter [mm]
      REQUIRE(std::get<0>(result.nodes[0].displacement).value() == Approx(0.0).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[0].displacement).value() == Approx(0.0).epsilon(kDivergence));
      REQUIRE(std::get<0>(result.nodes[1].displacement).value() == Approx(0.0).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[1].displacement).value() == Approx(0.0).epsilon(kDivergence));
      REQUIRE(std::get<0>(result.nodes[2].displacement).value() == Approx(-0.77).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[2].displacement).value() == Approx(-1.25).epsilon(kDivergence));
      REQUIRE(std::get<0>(result.nodes[3].displacement).value() == Approx(-0.77).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[3].displacement).value() == Approx(-0.8).epsilon(kDivergence));
      REQUIRE(std::get<0>(result.nodes[4].displacement).value() == Approx(0.77).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[4].displacement).value() == Approx(-0.8).epsilon(kDivergence));
      REQUIRE(std::get<0>(result.nodes[5].displacement).value() == Approx(0.77).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[5].displacement).value() == Approx(-1.25).epsilon(kDivergence));
      REQUIRE(std::get<0>(result.nodes[6].displacement).value() == Approx(-0.24).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[6].displacement).value() == Approx(-1.28).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[7].displacement).value() == Approx(-1.83).epsilon(kDivergence));

      // unit: newton [N]
      REQUIRE(result.members[0].normal_force.value() == Approx(-23'320).epsilon(kDivergence));
      REQUIRE(result.members[1].normal_force.value() == Approx(-23'320).epsilon(kDivergence));
      REQUIRE(result.members[2].normal_force.value() == Approx(-32'000));
      REQUIRE(result.members[3].normal_force.value() == Approx(-32'000));
      REQUIRE(result.members[6].normal_force.value() == Approx(16'970).epsilon(kDivergence));
      REQUIRE(result.members[7].normal_force.value() == Approx(16'970).epsilon(kDivergence));
      REQUIRE(result.members[8].normal_force.value() == Approx(-32'000));
      REQUIRE(result.members[9].normal_force.value() == Approx(-32'000));
      REQUIRE(result.members[12].normal_force.value() == Approx(12'000.0));
      REQUIRE(result.members[13].normal_force.value() == Approx(12'000.0));
   }
#endif
   SECTION("solve with eigen backend")
   {
      const auto load = sec21::read_from_json<loadcase<decltype(sys)>>("example_5_load.json");

      const auto success = solve<solver::backend::eigen>(sys, load);
      REQUIRE(success.has_value() == true);

      const auto result = success.value();

      // unit: newton [N]
      REQUIRE(std::get<0>(result.nodes[0].support_reaction).value() == Approx(-12'000).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[0].support_reaction).value() == Approx(52'000).epsilon(kDivergence));
      REQUIRE(std::get<0>(result.nodes[1].support_reaction).value() == Approx(12'000).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[1].support_reaction).value() == Approx(52'000).epsilon(kDivergence));

      // // unit: millimeter [mm]
      REQUIRE(std::get<0>(result.nodes[0].displacement).value() == Approx(0.0).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[0].displacement).value() == Approx(0.0).epsilon(kDivergence));
      REQUIRE(std::get<0>(result.nodes[1].displacement).value() == Approx(0.0).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[1].displacement).value() == Approx(0.0).epsilon(kDivergence));
      REQUIRE(std::get<0>(result.nodes[2].displacement).value() == Approx(-0.77).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[2].displacement).value() == Approx(-1.25).epsilon(kDivergence));
      REQUIRE(std::get<0>(result.nodes[3].displacement).value() == Approx(-0.77).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[3].displacement).value() == Approx(-0.8).epsilon(kDivergence));
      REQUIRE(std::get<0>(result.nodes[4].displacement).value() == Approx(0.77).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[4].displacement).value() == Approx(-0.8).epsilon(kDivergence));
      REQUIRE(std::get<0>(result.nodes[5].displacement).value() == Approx(0.77).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[5].displacement).value() == Approx(-1.25).epsilon(kDivergence));
      REQUIRE(std::get<0>(result.nodes[6].displacement).value() == Approx(-0.24).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[6].displacement).value() == Approx(-1.28).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[7].displacement).value() == Approx(-1.83).epsilon(kDivergence));

      // unit: newton [N]
      REQUIRE(result.members[0].normal_force.value() == Approx(-23'320).epsilon(kDivergence));
      REQUIRE(result.members[1].normal_force.value() == Approx(-23'320).epsilon(kDivergence));
      REQUIRE(result.members[2].normal_force.value() == Approx(-32'000));
      REQUIRE(result.members[3].normal_force.value() == Approx(-32'000));
      REQUIRE(result.members[6].normal_force.value() == Approx(16'970).epsilon(kDivergence));
      REQUIRE(result.members[7].normal_force.value() == Approx(16'970).epsilon(kDivergence));
      REQUIRE(result.members[8].normal_force.value() == Approx(-32'000));
      REQUIRE(result.members[9].normal_force.value() == Approx(-32'000));
      REQUIRE(result.members[12].normal_force.value() == Approx(12'000.0));
      REQUIRE(result.members[13].normal_force.value() == Approx(12'000.0));
   }
}