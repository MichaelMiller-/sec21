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

TEST_CASE("example system 1.0 with temperature load", "[sec21][structural_analysis][space_truss]")
{
   constexpr auto kDivergence{0.02};

   using namespace sec21::structural_analysis;
   using namespace sec21::units::literals;
   using namespace std::literals;

   using member_t = member<int, double>;
   using node_t = node<2, int, double>;
   using space_truss_t = space_truss<node_t, member_t>;

   auto sys = sec21::load_from_json<space_truss_t>("example_1.json");
   REQUIRE(std::size(sys.nodes) == 4);
   REQUIRE(std::size(sys.members) == 6);

   using loadcase_t = loadcase<decltype(sys)>;
   loadcase_t lf1;
   lf1.name = "LF1"s;
   lf1.temperature_loads.emplace_back(loadcase_t::temperature_load_t{1, 30.0_K, 0.012});

   auto [success, result] = solve<solver::backend::viennacl_impl>(sys, lf1);
   REQUIRE(success == true);

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

   // unit: millimeter [mm]
   REQUIRE(flat_displacement[0] == Approx(-0.54));
   REQUIRE(flat_displacement[1] == Approx(-0.112).epsilon(kDivergence));
   REQUIRE(flat_displacement[2] == Approx(0.428).epsilon(kDivergence));
   REQUIRE(flat_displacement[3] == Approx(-0.112).epsilon(kDivergence));
   REQUIRE(flat_displacement[4] == Approx(-0.112).epsilon(kDivergence));

   // unit: newton [N]
   REQUIRE(copied_results[0] == Approx(271'108).epsilon(kDivergence));
   //! \todo nachlagern / superpposionieren REQUIRE(copied_results[0] == Approx(-31'310).epsilon(kDivergence));
   REQUIRE(copied_results[1] == Approx(-31'300).epsilon(kDivergence));
   REQUIRE(copied_results[2] == Approx(-31'300).epsilon(kDivergence));
   //! \todo REQUIRE(copied_results[3] == Approx(5'000).epsilon(kDivergence));
   REQUIRE(copied_results[4] == Approx(44'300).epsilon(kDivergence));
   REQUIRE(copied_results[5] == Approx(44'300).epsilon(kDivergence));

   {
      std::ofstream ofs{"output_example_1_temp_result.json"};
      nlohmann::json tmp = result;
      ofs << std::setw(4) << tmp;
   }
}
