#include <catch.hpp>
#include "approx_equal.h"

#include <sec21/structural_analysis/loadcase.h>
#include <sec21/structural_analysis/space_truss.h>

#include <fstream>

TEST_CASE("test loadcase", "[sec21][structural_analysis][loadcase]")
{
   using namespace sec21::structural_analysis;
   using namespace sec21::units::literals;
   using namespace std::literals;

   std::ifstream ifs{"example_1.json"};
   using nlohmann::json;
   json j;
   ifs >> j;
   auto sys = j.get<space_truss>();

   REQUIRE(std::size(sys.nodes) == 4);

   using loadcase_t = loadcase<decltype(sys)>;
   constexpr auto dim = decltype(sys)::dimension_v;

   SECTION("create vector F from temperature loadcase")
   {
      loadcase_t lf1;
      lf1.name = "LF1"s;
      lf1.temperature_load.emplace_back(1, 30.0_K);

      using namespace sec21;
      using value_t = units::quantity<units::newton, typename decltype(sys)::precision_t>;

      std::vector<value_t> F(std::size(sys.nodes) * dim);
      REQUIRE(std::size(F) == 8);

      add_temperature_load(sys, lf1, F);

      const auto expected = std::array{ -302'400.0_N, 0.0_N, 302'400.0_N, 0.0_N, 0.0_N, 0.0_N, 0.0_N, 0.0_N };
      REQUIRE(std::size(F) == std::size(expected));
      REQUIRE(std::equal(std::begin(F), std::end(F), std::begin(expected)));
   }
   SECTION("create vector F from node load")
   {
      loadcase_t lf1;
      lf1.name = "LF1"s;
      lf1.node_load.push_back({ 2, {10.0_kN, -10.0_kN}});

      using namespace sec21;
      using value_t = units::quantity<units::newton, typename decltype(sys)::precision_t>;

      std::vector<value_t> F(std::size(sys.nodes) * dim);
      REQUIRE(std::size(F) == 8);

      add_node_load(sys, lf1, F);

      const auto expected = std::array{ 0.0_N, 0.0_N, 10'000.0_N, -10'000.0_N, 0.0_N, 0.0_N, 0.0_N, 0.0_N };
      REQUIRE(std::size(F) == std::size(expected));
      REQUIRE(std::equal(std::begin(F), std::end(F), std::begin(expected)));
   }
}
