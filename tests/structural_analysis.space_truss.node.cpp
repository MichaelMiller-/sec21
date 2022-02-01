#include <catch.hpp>

#include <sec21/structural_analysis/member.h>
#include <sec21/structural_analysis/node.h>
#include <sec21/structural_analysis/space_truss.h>

TEST_CASE("add node to system", "[sec21][structural_analysis][space_truss][node]")
{
   using namespace sec21::structural_analysis;

   using descriptor_t = int;
   using member_t = member<descriptor_t>;
   using node_t = node<2, descriptor_t>;
   using space_truss_t = space_truss<node_t, member_t>;

   SECTION("add one node")
   {
      auto sys = space_truss_t{};
      auto n1 = add_node(sys, 1);
      REQUIRE(n1 == 1);
   }
   SECTION("add node with invalid id")
   {
      REQUIRE_THROWS([]() {
         auto sys = space_truss_t{};
         auto r = add_node(sys, {std::numeric_limits<descriptor_t>::max()});
      }());
   }
   SECTION("add two nodes with the same id")
   {
      auto sys = space_truss_t{};
      auto n1 = add_node(sys, {1});

      REQUIRE(n1 == 1);
      REQUIRE_THROWS([&]() { add_node(sys, {1}); }());
   }
   SECTION("add one node via parameter pack")
   {
      auto sys = space_truss_t{};
      auto n1 = add_node(sys, descriptor_t{1});

      REQUIRE(n1 == 1);
   }
   SECTION("add one node via empty parameter pack")
   {
      REQUIRE_THROWS([&]() {
         auto sys = space_truss_t{};
         add_node(sys);
      }());
   }
}
