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
      REQUIRE(n1 == 1);
      REQUIRE(n2 == 2);

      auto m1 = add_member(sys, n1, n2, member_t{ 1, "", {}, "", "", 1.0, 1.0 });
      REQUIRE(m1 == 1);
   }
   SECTION("add member with invalid id")
   {
      auto sys = space_truss_t{};
      auto n1 = add_node(sys, { 1u });
      auto n2 = add_node(sys, { 2u });
      REQUIRE(n1 == 1);
      REQUIRE(n2 == 2);

      REQUIRE_THROWS([&](){
        auto m1 = add_member(sys, n1, n2, member_t{ std::numeric_limits<int>::max(), "", {}, "", "", 1.0, 1.0 });
      }());
   }
   SECTION("add two member with the same id")
   {
      auto sys = space_truss_t{};
      auto n1 = add_node(sys, { 1u });
      auto n2 = add_node(sys, { 2u });
      REQUIRE(n1 == 1);
      REQUIRE(n2 == 2);

      auto m1 = add_member(sys, n1, n2, member_t{ 1u, "", {}, "", "", 1.0, 1.0 });
      REQUIRE(m1 == 1);

      REQUIRE_THROWS([&](){
        add_member(sys, n1, n2, member_t{ 1u, "", {}, "", "", 1.0, 1.0 });
      }());
   }
   SECTION("add one member via parameter pack")
   {
      auto sys = space_truss_t{};
      auto n1 = add_node(sys, { 1u });
      auto n2 = add_node(sys, { 2u });
      REQUIRE(n1 == 1);
      REQUIRE(n2 == 2);

      auto m1 = add_member(sys, n1, n2, decltype(sys)::member_descriptor_t{ 1 });
      REQUIRE(m1 == 1);
   }
   SECTION("add one member via empty parameter pack")
   {
      auto sys = space_truss_t{};
      auto n1 = add_node(sys, { 1u });
      auto n2 = add_node(sys, { 2u });
      REQUIRE(n1== 1);
      REQUIRE(n2 == 2);

      REQUIRE_THROWS([&](){
        add_member(sys, n1, n2);
      }());
   }
   SECTION("add member to a invalid node")
   {
      auto sys = space_truss_t{};
      auto n1 = add_node(sys, { 1u });
      REQUIRE(n1 == 1);

      REQUIRE_THROWS([&](){
        add_member(sys, n1, 42);
      }());
   }
}
