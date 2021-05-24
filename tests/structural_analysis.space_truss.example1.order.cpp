#include <catch.hpp>
#include "approx_equal.h"

#include <sec21/numeric/flatten.h>
#include <sec21/numeric/ublas_allocator_wrapper.h>
#include <sec21/structural_analysis/loadcase.h>
#include <sec21/structural_analysis/solve.h>
#include <sec21/structural_analysis/node.h>
#include <sec21/structural_analysis/member.h>
#include <sec21/structural_analysis/space_truss.h>
#include <sec21/structural_analysis/system_result.h>
#include <sec21/units.h>
#include <sec21/structural_analysis/solver/backend/viennacl.h>

TEST_CASE("example 1.1 with different node id's (N1 switched with N3 from example 1)", "[sec21][structural_analysis][space_truss]")
{
   constexpr auto kDivergence{0.02};

   using namespace sec21;
   using namespace sec21::structural_analysis;
   using namespace sec21::units::literals;

   using member_t = member<int, double>;
   using node_t = node<2, int, double>;
   using space_truss_t = space_truss<node_t, member_t>;

   auto sys = space_truss_t{};
   using precision_t = decltype(sys)::precision_t;

   using precision_t = decltype(sys)::precision_t;
   using node_t = decltype(sys)::node_t;
   using support_t = node_t::global_support_t;

   auto n1 = add_node(sys, {1, {3.0, 0.0}, support_t{ false, true }});
   auto n2 = add_node(sys, {2, {3.0, 3.0}});
   auto n3 = add_node(sys, {3, {0.0, 3.0}});
   auto n4 = add_node(sys, {4, {0.0, 0.0}, support_t{ true, true }});

   REQUIRE(static_cast<bool>(n1) == true);
   REQUIRE(static_cast<bool>(n2) == true);
   REQUIRE(static_cast<bool>(n3) == true);
   REQUIRE(static_cast<bool>(n4) == true);

   auto m1 = add_member(sys, 3, 2, {1, 0.004, 21'000'000});
   auto m2 = add_member(sys, 1, 2, {2, 0.004, 21'000'000});
   auto m3 = add_member(sys, 4, 1, {3, 0.004, 21'000'000});
   auto m4 = add_member(sys, 4, 3, {4, 0.004, 21'000'000});
   auto m5 = add_member(sys, 4, 2, {5, 0.004, 21'000'000});
   auto m6 = add_member(sys, 1, 3, {6, 0.004, 21'000'000});

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
      REQUIRE(impl::length(sys, m5.value()).value() == Approx(4.24264)); //_m);
      REQUIRE(impl::length(sys, m6.value()).value() == Approx(4.24264)); //_m);

      namespace bmc = boost::math::constants;
      const auto fourth_pi{bmc::half_pi<precision_t>() * 0.5};

      REQUIRE(impl::angle_to_x_axis(sys, m1.value()) == Approx(0.0));
      REQUIRE(impl::angle_to_x_axis(sys, m2.value()) == Approx(bmc::half_pi<precision_t>()));
      REQUIRE(impl::angle_to_x_axis(sys, m3.value()) == Approx(0.0));
      REQUIRE(impl::angle_to_x_axis(sys, m4.value()) == Approx(bmc::half_pi<precision_t>()));
      REQUIRE(impl::angle_to_x_axis(sys, m5.value()) == Approx(fourth_pi));
      REQUIRE(impl::angle_to_x_axis(sys, m6.value()) == Approx(-fourth_pi));
   }
   SECTION("filter supported nodes from lookup table") 
   {
      auto lookup = impl::make_lookup(sys, row{0});

      const auto expected = std::array{ row{0}, row{1}, row{2}, row{3}, row{4}, row{5}, row{6}, row{7} };
      REQUIRE(std::size(lookup) == std::size(expected));
      REQUIRE(std::equal(std::begin(lookup), std::end(lookup), std::begin(expected)));

      std::vector<bool> mask;
      support_mask(std::begin(sys.nodes), std::end(sys.nodes), std::back_inserter(mask));

      decltype(lookup) supported_nodes;
      decltype(lookup) not_supported_nodes;

      impl::partition_lookup(
         std::begin(lookup), 
         std::end(lookup),
         std::begin(mask), 
         std::end(mask),
         std::back_inserter(supported_nodes),
         std::back_inserter(not_supported_nodes));

      const auto expected_rows = std::vector{ row{1}, row{6}, row{7} };

      REQUIRE(std::size(supported_nodes) == std::size(expected_rows));
      REQUIRE(std::equal(std::begin(supported_nodes), std::end(supported_nodes), std::begin(expected_rows)));
   }
   SECTION("coincidence matrix from member 1")
   {
      using allocator_t = sec21::numeric::ublas_allocator_wrapper<std::allocator<double>>;
      auto Z = impl::coincidence_matrix<allocator_t>(sys, m1.value());
      REQUIRE(Z.size1() == 4);
      REQUIRE(Z.size2() == 8);
      //! \clang-format off
      const auto expected = std::array{
         0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0,
         0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0,
      };
      //! \clang-format on
      REQUIRE(approx_equal(sec21::numeric::flatten(Z), expected, kDivergence));
   }
   SECTION("coincidence matrix from member 2")
   {
      using allocator_t = sec21::numeric::ublas_allocator_wrapper<std::allocator<double>>;
      auto Z = impl::coincidence_matrix<allocator_t>(sys, m2.value());
      REQUIRE(Z.size1() == 4);
      REQUIRE(Z.size2() == 8);
      //! \clang-format off
      const auto expected = std::array{
         1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0,
      };
      //! \clang-format on
      REQUIRE(approx_equal(sec21::numeric::flatten(Z), expected, kDivergence));
   }
   SECTION("coincidence matrix from member 3")
   {
      using allocator_t = sec21::numeric::ublas_allocator_wrapper<std::allocator<double>>;
      auto Z = impl::coincidence_matrix<allocator_t>(sys, m3.value());
      REQUIRE(Z.size1() == 4);
      REQUIRE(Z.size2() == 8);
      //! \clang-format off
      const auto expected = std::array{
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0,
         1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      };
      //! \clang-format on
      REQUIRE(approx_equal(sec21::numeric::flatten(Z), expected, kDivergence));
   }
   SECTION("coincidence matrix from member 4")
   {
      using allocator_t = sec21::numeric::ublas_allocator_wrapper<std::allocator<double>>;
      auto Z = impl::coincidence_matrix<allocator_t>(sys, m4.value());
      REQUIRE(Z.size1() == 4);
      REQUIRE(Z.size2() == 8);
      //! \clang-format off
      const auto expected = std::array{
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0,
         0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0,
      };
      //! \clang-format on
      REQUIRE(approx_equal(sec21::numeric::flatten(Z), expected, kDivergence));
   }     
   SECTION("coincidence matrix from member 5")
   {
      using allocator_t = sec21::numeric::ublas_allocator_wrapper<std::allocator<double>>;
      auto Z = impl::coincidence_matrix<allocator_t>(sys, m5.value());
      REQUIRE(Z.size1() == 4);
      REQUIRE(Z.size2() == 8);
      //! \clang-format off
      const auto expected = std::array{
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0,
         0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0,
      };
      //! \clang-format on
      REQUIRE(approx_equal(sec21::numeric::flatten(Z), expected, kDivergence));
   }
   SECTION("coincidence matrix from member 6")
   {
      using allocator_t = sec21::numeric::ublas_allocator_wrapper<std::allocator<double>>;
      auto Z = impl::coincidence_matrix<allocator_t>(sys, m6.value());
      REQUIRE(Z.size1() == 4);
      REQUIRE(Z.size2() == 8);
      //! \clang-format off
      const auto expected = std::array{
         1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0,
      };
      //! \clang-format on
      REQUIRE(approx_equal(sec21::numeric::flatten(Z), expected, kDivergence));
   }   
   SECTION("solve")
   {
      loadcase<decltype(sys)> lf1;
      lf1.node_load.emplace_back(2, loadcase<decltype(sys)>::load_t{{10.0_kN, -10.0_kN}});

      auto [success, result] = solve<solver::backend::viennacl_impl>(sys, lf1);
      REQUIRE(success == true);

      std::vector<double> flat_support_reaction{};
      for (auto [k,v] : result.node) 
      {
         std::transform(
            std::begin(v.support_reaction), 
            std::end(v.support_reaction), 
            std::back_inserter(flat_support_reaction),
            [](auto&& e) { return e.value(); });
      }

      std::vector<double> flat_displacement{};
      for (auto [k,v] : result.node) 
      {
         std::transform(
            std::begin(v.displacement), 
            std::end(v.displacement), 
            std::back_inserter(flat_displacement),
            [](auto&& e) { return e.value(); });
      }

      std::vector<double> flat_member_result{};
      std::transform(
         std::begin(result.members), 
         std::end(result.members), 
         std::back_inserter(flat_member_result),
         [](auto&& e) { return e.normal_force.value(); });

      // unit: newton [N]
      REQUIRE(flat_support_reaction[0] == Approx(0.0));
      REQUIRE(flat_support_reaction[1] == Approx(20'000.0));
      REQUIRE(flat_support_reaction[2] == Approx(0.0));
      REQUIRE(flat_support_reaction[3] == Approx(0.0));
      REQUIRE(flat_support_reaction[4] == Approx(0.0));
      REQUIRE(flat_support_reaction[5] == Approx(0.0));
      REQUIRE(flat_support_reaction[6] == Approx(-10'000.0));
      REQUIRE(flat_support_reaction[7] == Approx(-10'000.0));

      // unit: millimeter [mm]
      REQUIRE(flat_displacement[0] == Approx(0.18).epsilon(kDivergence));
      REQUIRE(flat_displacement[1] == Approx(0.0));
      REQUIRE(flat_displacement[2] == Approx(1.04).epsilon(kDivergence));
      REQUIRE(flat_displacement[3] == Approx(-0.54).epsilon(kDivergence));
      REQUIRE(flat_displacement[4] == Approx(0.86).epsilon(kDivergence));
      REQUIRE(flat_displacement[5] == Approx(0.18).epsilon(kDivergence));
      REQUIRE(flat_displacement[6] == Approx(0.0));
      REQUIRE(flat_displacement[7] == Approx(0.0));

      // unit: newton [N])
      REQUIRE(flat_member_result[0] == Approx(5'000));
      REQUIRE(flat_member_result[1] == Approx(-15'000));
      REQUIRE(flat_member_result[2] == Approx(5'000));
      REQUIRE(flat_member_result[3] == Approx(5'000));
      REQUIRE(flat_member_result[4] == Approx(7'071.0678118655));
      REQUIRE(flat_member_result[5] == Approx(-7'071.0678118655));
   }
}
