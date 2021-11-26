﻿#include <catch.hpp>

#include <sec21/structural_analysis/loadcase.h>
#include <sec21/structural_analysis/solve.h>
#include <sec21/structural_analysis/node.h>
#include <sec21/structural_analysis/member.h>
#include <sec21/structural_analysis/space_truss.h>
#include <sec21/units.h>
#include <sec21/structural_analysis/solver/backend/eigen.h>

#include <vector>
#include <algorithm>

TEST_CASE("example system 1.2 with half load from example 1.0", "[sec21][structural_analysis][space_truss]")
{
   constexpr auto kDivergence{0.02};

   using namespace sec21::structural_analysis;
   using namespace sec21::units::literals;

   using precision_t = float;
   using member_t = member<int, precision_t>;
   using node_t = node<2, int, precision_t>;
   using support_t = node_t::global_support_t;
   using space_truss_t = space_truss<node_t, member_t>;

   auto sys = space_truss_t{};

   auto n1 = add_node(sys, node_t{1, {0.0, 3.0}});
   auto n2 = add_node(sys, node_t{2, {3.0, 3.0}});
   auto n3 = add_node(sys, node_t{3, {3.0, 0.0}, support_t{ false, true }});
   auto n4 = add_node(sys, node_t{4, {0.0, 0.0}, support_t{ true, true }});

   REQUIRE(static_cast<bool>(n1) == true);
   REQUIRE(static_cast<bool>(n2) == true);
   REQUIRE(static_cast<bool>(n3) == true);
   REQUIRE(static_cast<bool>(n4) == true);

   // constexpr auto E = 210.0_kPa;
   // constexpr auto A = unit::square_meter<double>(0.004);
   auto m1 = add_member(sys, 1, 2, member_t{1, "", {}, "", "", 0.004, 21'000'000});
   auto m2 = add_member(sys, 3, 2, member_t{2, "", {}, "", "", 0.004, 21'000'000});
   auto m3 = add_member(sys, 4, 3, member_t{3, "", {}, "", "", 0.004, 21'000'000});
   auto m4 = add_member(sys, 4, 1, member_t{4, "", {}, "", "", 0.004, 21'000'000});
   auto m5 = add_member(sys, 4, 2, member_t{5, "", {}, "", "", 0.004, 21'000'000});
   auto m6 = add_member(sys, 3, 1, member_t{6, "", {}, "", "", 0.004, 21'000'000});

   REQUIRE(static_cast<bool>(m1) == true);
   REQUIRE(static_cast<bool>(m2) == true);
   REQUIRE(static_cast<bool>(m3) == true);
   REQUIRE(static_cast<bool>(m4) == true);
   REQUIRE(static_cast<bool>(m5) == true);
   REQUIRE(static_cast<bool>(m6) == true);

   SECTION("test the geometry of the system")
   {
      REQUIRE(impl::length(sys, m1.value()) == 3.0_m);
      REQUIRE(impl::length(sys, m2.value()) == 3.0_m);
      REQUIRE(impl::length(sys, m3.value()) == 3.0_m);
      REQUIRE(impl::length(sys, m4.value()) == 3.0_m);
      REQUIRE(impl::length(sys, m5.value()).value() == Approx(4.24264).epsilon(kDivergence)); //_m);
      REQUIRE(impl::length(sys, m6.value()).value() == Approx(4.24264).epsilon(kDivergence)); //_m);

      namespace bmc = boost::math::constants;
      const auto fourth_pi{bmc::half_pi<precision_t>() * static_cast<precision_t>(0.5)};

      REQUIRE(impl::angle_to_x_axis(sys, m1.value()) == Approx(0.0));
      REQUIRE(impl::angle_to_x_axis(sys, m2.value()) == Approx(bmc::half_pi<precision_t>()));
      REQUIRE(impl::angle_to_x_axis(sys, m3.value()) == Approx(0.0));
      REQUIRE(impl::angle_to_x_axis(sys, m4.value()) == Approx(bmc::half_pi<precision_t>()));
      REQUIRE(impl::angle_to_x_axis(sys, m5.value()) == Approx(fourth_pi));
      REQUIRE(impl::angle_to_x_axis(sys, m6.value()) == Approx(-fourth_pi));
   }
   SECTION("solve")
   {
      using namespace sec21;

      loadcase<decltype(sys)> lf1{};
      lf1.node_load.emplace_back(2, loadcase<decltype(sys)>::load_t{{5.0_kN, -5.0_kN}});

      const auto success = solve<solver::backend::eigen>(sys, lf1);
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
      std::vector<double> flat_member_result{};
      std::transform(
         std::begin(result.members), 
         std::end(result.members), 
         std::back_inserter(flat_member_result),
         [](auto&& e) { return e.normal_force.value(); });
/*
      // unit: newton [N]
      REQUIRE(flat_support_reaction[0] == Approx(0.0));
      REQUIRE(flat_support_reaction[1] == Approx(0.0));
      REQUIRE(flat_support_reaction[2] == Approx(0.0));
      REQUIRE(flat_support_reaction[3] == Approx(0.0));
      REQUIRE(flat_support_reaction[4] == Approx(0.0));
      REQUIRE(flat_support_reaction[5] == Approx(10'000.0));
      REQUIRE(flat_support_reaction[6] == Approx(-5'000.0));
      REQUIRE(flat_support_reaction[7] == Approx(-5'000.0));
*/

      // unit: millimeter [mm]
      REQUIRE(std::get<0>(result.nodes[0].displacement).value() == Approx(0.43).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[0].displacement).value() == Approx(0.09).epsilon(kDivergence));
      REQUIRE(std::get<0>(result.nodes[1].displacement).value() == Approx(0.52).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[1].displacement).value() == Approx(-0.27).epsilon(kDivergence));
      REQUIRE(std::get<0>(result.nodes[2].displacement).value() == Approx(0.09).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[2].displacement).value() == Approx(0.0).epsilon(kDivergence));
      REQUIRE(std::get<0>(result.nodes[3].displacement).value() == Approx(0.0).epsilon(kDivergence));
      REQUIRE(std::get<1>(result.nodes[3].displacement).value() == Approx(0.0).epsilon(kDivergence));

      // unit: newton [N])
      REQUIRE(flat_member_result[0] == Approx(2'500));
      REQUIRE(flat_member_result[1] == Approx(-7'500));
      REQUIRE(flat_member_result[2] == Approx(2'500));
      REQUIRE(flat_member_result[3] == Approx(2'500));
      REQUIRE(flat_member_result[4] == Approx(3'535.53).epsilon(kDivergence));
      REQUIRE(flat_member_result[5] == Approx(-3'535.53).epsilon(kDivergence));
   }
}