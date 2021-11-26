#include <catch.hpp>

#include <sec21/structural_analysis/node.h>
#include <sec21/structural_analysis/member.h>
#include <sec21/structural_analysis/space_truss.h>

TEST_CASE("add node to system", "[sec21][structural_analysis][space_truss][node]")
{
   using namespace sec21::structural_analysis;

   using member_t = member<int, float>;
   using node_t = node<2, int, float>;
   using space_truss_t = space_truss<node_t, member_t>;

   SECTION("add one node")
   {
      auto sys = space_truss_t{};
      auto n1 = add_node(sys, size_t(1));

      REQUIRE(static_cast<bool>(n1) == true);
      REQUIRE(n1.value() == 1);
   }
   SECTION("add node with invalid id")
   {
      auto sys = space_truss_t{};
      auto n1 = add_node(sys, { std::numeric_limits<decltype(sys)::node_t::descriptor_t>::max() });

      REQUIRE(static_cast<bool>(n1) == false);
   }
   SECTION("add two nodes with the same id")
   {
      auto sys = space_truss_t{};
      auto n1 = add_node(sys, { 1u });
      auto n2 = add_node(sys, { 1u });

      REQUIRE(static_cast<bool>(n1) == true);
      REQUIRE(n1.value() == 1);
      REQUIRE(static_cast<bool>(n2) == false);
   }
   SECTION("add one node via parameter pack")
   {
      auto sys = space_truss_t{};
      auto n1 = add_node(sys, decltype(sys)::node_descriptor_t{ 1 });

      REQUIRE(static_cast<bool>(n1) == true);
      REQUIRE(n1.value() == 1);
   }
   SECTION("add one node via empty parameter pack")
   {
      auto sys = space_truss_t{};
      auto n1 = add_node(sys);

      REQUIRE(static_cast<bool>(n1) == false);
   }
}
