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

TEST_CASE("example system 5.0 load from json", "[sec21][structural_analysis][space_truss]")
{
   constexpr auto kDivergence{0.01};

   using namespace sec21::structural_analysis;
   using namespace sec21::units::literals;

   using member_t = member<int, double>;
   using node_t = node<2, int, double>;
   using space_truss_t = space_truss<node_t, member_t>;

   auto sys = sec21::load_from_json<space_truss_t>("example_5.json");

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
   SECTION("E*A units from first member")
   {
      auto it = std::begin(sys.members);
      auto EA = it->E * it->A;
      REQUIRE(EA.value() == Approx(200000));
   }
   SECTION("solve")
   {
      auto load = sec21::load_from_json<loadcase<decltype(sys)>>("example_5_load.json");

      const auto success = solve<solver::backend::viennacl_impl>(sys, load);
      REQUIRE(success.has_value() == true);

      const auto result = success.value();

      std::vector<double> flat_support_reaction{};
      for (auto [k,v] : result.node) 
      {
         std::transform(
            std::begin(v.support_reaction), 
            std::end(v.support_reaction), 
            std::back_inserter(flat_support_reaction),
            [](auto&& e) { return e.value(); });
      }


      std::vector<double> copied_results{};
      std::transform(
         std::begin(result.members), 
         std::end(result.members), 
         std::back_inserter(copied_results), 
         [](auto&& m) { return m.normal_force.value(); });

      // unit: newton [N]
      REQUIRE(std::get<0>(result.nodes[0].support_reaction).value() == Approx(-12'000).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[0].support_reaction).value() == Approx(52'000).epsilon(kDivergence));
      REQUIRE(std::get<0>(result.nodes[1].support_reaction).value() == Approx(12'000).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[1].support_reaction).value() == Approx(52'000).epsilon(kDivergence));

      // // unit: millimeter [mm]
      REQUIRE(std::get<1>(result.nodes[7].displacement).value() == Approx(-1.83).epsilon(kDivergence));

      // unit: newton [N]
      REQUIRE(copied_results[0] == Approx(-23'320).epsilon(kDivergence));
      REQUIRE(copied_results[1] == Approx(-23'320).epsilon(kDivergence));
      REQUIRE(copied_results[2] == Approx(-32'000));
      REQUIRE(copied_results[3] == Approx(-32'000));
      REQUIRE(copied_results[6] == Approx(16'970).epsilon(kDivergence));
      REQUIRE(copied_results[7] == Approx(16'970).epsilon(kDivergence));
      REQUIRE(copied_results[8] == Approx(-32'000));
      REQUIRE(copied_results[9] == Approx(-32'000));
      REQUIRE(copied_results[12] == Approx(12'000.0));
      REQUIRE(copied_results[13] == Approx(12'000.0));

      {
         std::ofstream ofs{"output_example_5_result.json"};
         nlohmann::json tmp = result;
         ofs << std::setw(4) << tmp;
      }         
   }
}