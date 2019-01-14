#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <limits>

#include <sec21/strong_type.h>
#include <sec21/policies/comparable.h>
#include <sec21/policies/printable.h>
#include <sec21/policies/sortable.h>

TEST_CASE("test strong types", "[strong_type]")
{
   using namespace sec21;

   using node_id_t = strong_type<size_t, struct node_id_tag, comparable, printable>;
   using member_id_t = strong_type<size_t, struct member_id_tag, comparable, printable, sortable>;

   // constants
   constexpr node_id_t invalid_node_id{ std::numeric_limits<node_id_t::type>::max() };
   constexpr member_id_t invalid_member_id{ std::numeric_limits<member_id_t::type>::max() };

   node_id_t id{ 5 };
   REQUIRE(id == node_id_t{ 5 });

   node_id_t baz{ invalid_node_id };
   std::cout << id << std::endl;

   // interface test
   auto foo = [](node_id_t id) { return id == node_id_t{7}; };

   REQUIRE(foo(id) == false);
   REQUIRE(foo(node_id_t{3}) == false);

   member_id_t id2{ invalid_member_id };
}
