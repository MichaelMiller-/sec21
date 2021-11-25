#include <catch.hpp>

#include <sec21/file_loader.h>
#include <sec21/structural_analysis/loadcase.h>
#include <sec21/structural_analysis/node.h>
#include <sec21/structural_analysis/member.h>
#include <sec21/structural_analysis/space_truss.h>

TEST_CASE("test loadcase", "[sec21][structural_analysis][loadcase]")
{
   using namespace sec21::structural_analysis;
   using namespace sec21::units::literals;
   using namespace std::literals;

   using member_t = member<int, double>;
   using node_t = node<2, int, double>;
   using space_truss_t = space_truss<node_t, member_t>;

   auto sys = sec21::read_from_json<space_truss_t>("example_1.json");
   REQUIRE(std::size(sys.nodes) == 4);

   using loadcase_t = loadcase<decltype(sys)>;
   constexpr auto dim = decltype(sys)::dimension_v;

   SECTION("create vector F from temperature loadcase")
   {
      loadcase_t lf1;
      lf1.name = "LF1"s;
      lf1.temperature_loads.emplace_back(loadcase_t::temperature_load_t{1, 30.0_K, 0.012});

      using namespace sec21;
      using value_t = units::quantity<units::newton, typename decltype(sys)::precision_t>;

      std::vector<value_t> F(std::size(sys.nodes) * dim);
      REQUIRE(std::size(F) == 8);

      add_temperature_load(sys, lf1, begin(F));

      const auto expected = std::array{ -30'240.0_N, 0.0_N, 30'240.0_N, 0.0_N, 0.0_N, 0.0_N, 0.0_N, 0.0_N };
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
   SECTION("create vector F from multiply node loads")
   {
      loadcase_t lf1;
      lf1.name = "LF1"s;
      lf1.node_load.push_back({ 2, { 10.0_kN, -10.0_kN}});
      lf1.node_load.push_back({ 4, {  5.0_kN,  4.0_kN}});
      lf1.node_load.push_back({ 1, { -7.0_kN,  7.0_kN}});

      using namespace sec21;
      using value_t = units::quantity<units::newton, typename decltype(sys)::precision_t>;

      std::vector<value_t> F(std::size(sys.nodes) * dim);
      REQUIRE(std::size(F) == 8);

      add_node_load(sys, lf1, F);

      const auto expected = std::array{ -7000.0_N, 7000.0_N, 10'000.0_N, -10'000.0_N, 0.0_N, 0.0_N, 5000.0_N, 4000.0_N };
      REQUIRE(std::size(F) == std::size(expected));
      REQUIRE(std::equal(std::begin(F), std::end(F), std::begin(expected)));
   }   
}
