#include <catch.hpp>

#include <sec21/structural_analysis/node.h>
#include <sec21/structural_analysis/member.h>
#include <sec21/structural_analysis/space_truss.h>

#include <limits>

TEST_CASE("add member to system", "[sec21][structural_analysis][space_truss]")
{
   using namespace sec21::structural_analysis;

   using member_t = member<int, double>;
   using node_t = node<2, int, double>;
   using space_truss_t = space_truss<node_t, member_t>;

   SECTION("add one member")
   {
      auto sys = space_truss_t{};
      auto n1 = add_node(sys, { 1u });
      auto n2 = add_node(sys, { 2u });
      REQUIRE(static_cast<bool>(n1) == true);
      REQUIRE(n1.value() == 1);
      REQUIRE(static_cast<bool>(n2) == true);
      REQUIRE(n2.value() == 2);

      auto m1 = add_member(sys, n1.value(), n2.value(), { 1, "", {}, "", "", 1.0, 1.0 });
      REQUIRE(static_cast<bool>(m1) == true);
      REQUIRE(m1.value() == 1);
   }
   SECTION("add member with invalid id")
   {
      auto sys = space_truss_t{};
      auto n1 = add_node(sys, { 1u });
      auto n2 = add_node(sys, { 2u });
      REQUIRE(static_cast<bool>(n1) == true);
      REQUIRE(n1.value() == 1);
      REQUIRE(static_cast<bool>(n2) == true);
      REQUIRE(n2.value() == 2);

      auto m1 = add_member(sys, n1.value(), n2.value(), { std::numeric_limits<int>::max(), "", {}, "", "", 1.0, 1.0 });
      REQUIRE(static_cast<bool>(m1) == false);
   }
   SECTION("add two member with the same id")
   {
      auto sys = space_truss_t{};
      auto n1 = add_node(sys, { 1u });
      auto n2 = add_node(sys, { 2u });
      REQUIRE(static_cast<bool>(n1) == true);
      REQUIRE(n1.value() == 1);
      REQUIRE(static_cast<bool>(n2) == true);
      REQUIRE(n2.value() == 2);

      auto m1 = add_member(sys, n1.value(), n2.value(), { 1u, "", {}, "", "", 1.0, 1.0 });
      auto m2 = add_member(sys, n1.value(), n2.value(), { 1u, "", {}, "", "", 1.0, 1.0 });
      REQUIRE(static_cast<bool>(m1) == true);
      REQUIRE(m1.value() == 1);
      REQUIRE(static_cast<bool>(m2) == false);
   }
   SECTION("add one member via parameter pack")
   {
      auto sys = space_truss_t{};
      auto n1 = add_node(sys, { 1u });
      auto n2 = add_node(sys, { 2u });
      REQUIRE(static_cast<bool>(n1) == true);
      REQUIRE(n1.value() == 1);
      REQUIRE(static_cast<bool>(n2) == true);
      REQUIRE(n2.value() == 2);

      auto m1 = add_member(sys, n1.value(), n2.value(), decltype(sys)::member_descriptor_t{ 1 });
      REQUIRE(static_cast<bool>(m1) == true);
      REQUIRE(m1.value() == 1);
   }
   SECTION("add one member via empty parameter pack")
   {
      auto sys = space_truss_t{};
      auto n1 = add_node(sys, { 1u });
      auto n2 = add_node(sys, { 2u });
      REQUIRE(static_cast<bool>(n1) == true);
      REQUIRE(n1.value() == 1);
      REQUIRE(static_cast<bool>(n2) == true);
      REQUIRE(n2.value() == 2);

      auto m1 = add_member(sys, n1.value(), n2.value());
      REQUIRE(static_cast<bool>(m1) == false);
   }
   SECTION("add member to a invalid node")
   {
      auto sys = space_truss_t{};
      auto n1 = add_node(sys, { 1u });
      REQUIRE(static_cast<bool>(n1) == true);
      REQUIRE(n1.value() == 1);

      auto m1 = add_member(sys, n1.value(), 42);
      REQUIRE(static_cast<bool>(m1) == false);
   }
}
