#include <catch.hpp>

#include <sec21/file_loader.h>
#include <sec21/structural_analysis/node.h>
#include <sec21/structural_analysis/member.h>
#include <sec21/structural_analysis/space_truss.h>
#include <sec21/structural_analysis/loadcase.h>
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

   REQUIRE(size(sys.nodes) == 4);
   REQUIRE(size(sys.members) == 6);
   REQUIRE(size(sys.coincidence_table) == 6);

   using loadcase_t = loadcase<decltype(sys)>;
   loadcase_t lf1;
   lf1.name = "LF1"s;
   lf1.temperature_loads.emplace_back(loadcase_t::temperature_load_t{1, 30.0_K, 0.012});

   SECTION("create vector F from temperature loadcase")
   {
      constexpr auto dim = decltype(sys)::dimension_v;

      using namespace sec21;
      using value_t = units::quantity<units::newton, typename decltype(sys)::precision_t>;

      std::vector<value_t> F(std::size(sys.nodes) * dim);
      REQUIRE(std::size(F) == 8);

      add_temperature_load(sys, lf1, begin(F));

      const auto expected = std::array{ -30'240.0_N, 0.0_N, 30'240.0_N, 0.0_N, 0.0_N, 0.0_N, 0.0_N, 0.0_N };
      REQUIRE(std::size(F) == std::size(expected));
      REQUIRE(std::equal(std::begin(F), std::end(F), std::begin(expected)));
   }

   const auto success = solve<solver::backend::viennacl_impl>(sys, lf1);
   REQUIRE(success.has_value() == true);

   const auto result = success.value();

   // unit: millimeter [mm]
   REQUIRE(std::get<0>(result.nodes[0].displacement).value() == Approx(-0.54).epsilon(kDivergence));
   REQUIRE(std::get<1>(result.nodes[0].displacement).value() == Approx(-0.112).epsilon(kDivergence));
   REQUIRE(std::get<0>(result.nodes[1].displacement).value() == Approx(0.428).epsilon(kDivergence));
   REQUIRE(std::get<1>(result.nodes[1].displacement).value() == Approx(-0.112).epsilon(kDivergence));
   REQUIRE(std::get<0>(result.nodes[2].displacement).value() == Approx(-0.112).epsilon(kDivergence));

   // unit: newton [N]
   REQUIRE(result.members[0].normal_force.value() == Approx(27'108).epsilon(kDivergence));
   REQUIRE(result.members[1].normal_force.value() == Approx(-3'130).epsilon(kDivergence));
   REQUIRE(result.members[2].normal_force.value() == Approx(-3'130).epsilon(kDivergence));
   REQUIRE(result.members[3].normal_force.value() == Approx(-3'130).epsilon(kDivergence));
   REQUIRE(result.members[4].normal_force.value() == Approx(4'430).epsilon(kDivergence));
   REQUIRE(result.members[5].normal_force.value() == Approx(4'430).epsilon(kDivergence));
}
