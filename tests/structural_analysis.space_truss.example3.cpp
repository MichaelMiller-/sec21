#include <catch.hpp>

#include <sec21/file_loader.h>
#include <sec21/structural_analysis/node.h>
#include <sec21/structural_analysis/member.h>
#include <sec21/structural_analysis/space_truss.h>
#include <sec21/structural_analysis/loadcase.h>
#include <sec21/structural_analysis/solve.h>
#include <sec21/structural_analysis/solver/backend/viennacl.h>

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
   SECTION("solve")
   {
      auto lf1 = sec21::load_from_json<loadcase<decltype(sys)>>("example_3_load.json");

      const auto success = solve<solver::backend::viennacl_impl>(sys, lf1);
      REQUIRE(success.has_value() == true);

      const auto result = success.value();

      std::vector<double> copied_results{};
      std::transform(
         std::begin(result.members), 
         std::end(result.members), 
         std::back_inserter(copied_results), 
         [](auto&& m) { return m.normal_force.value(); });

      std::for_each(begin(result.nodes), end(result.nodes), [](auto n) {
        std::cout << "n: " << n.id << " x:" << std::get<0>(n.support_reaction).value()
                  << " y: " << std::get<1>(n.support_reaction).value() << std::endl;
      });

      // unit: newton [N]
      REQUIRE(std::get<0>(result.nodes[0].support_reaction).value() == Approx(68'000).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[0].support_reaction).value() == Approx(76'000).epsilon(kDivergence));
      REQUIRE(std::get<0>(result.nodes[1].support_reaction).value() == Approx(60'000).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[1].support_reaction).value() == Approx(-76'000).epsilon(kDivergence));

      // unit: millimeter [mm]
      REQUIRE(std::get<0>(result.nodes[5].displacement).value() == Approx(-26.7).epsilon(kDivergence));

      // unit: newton [N]
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