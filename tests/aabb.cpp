#include <catch.hpp>

#include <sec21/aabb.h>
#include <sec21/structural_analysis/shape/cross_section_vertex.h>

TEST_CASE("Axis aligned bounding box class (aabb)", "[sec21]")
{
   using namespace sec21;
   using vertex_t = structural_analysis::shape::cross_section_vertex<int>;

   SECTION("calculate from vertices")
   {
      const auto input= std::vector<vertex_t>{
         {0,0},
         {10,10},
      };
      const auto result = bounding_rect(input);
      REQUIRE(result.min == vertex_t{0,0});
      REQUIRE(result.max == vertex_t{10,10});
   }
   SECTION("calculate from triangle vertices")
   {
      const auto input= std::vector<vertex_t>{
         {0,0},
         {20,00},
         {10,10},
      };
      const auto result = bounding_rect(input);
      REQUIRE(result.min == vertex_t{0,0});
      REQUIRE(result.max == vertex_t{20,10});
   }
   SECTION("calculate from a negative moved triangle")
   {
      const auto input= std::vector<vertex_t>{
         {-10,-50},
         {20, -70},
         {10,10},
      };
      const auto result = bounding_rect(input);
      REQUIRE(result.min == vertex_t{-10,-70});
      REQUIRE(result.max == vertex_t{20,10});
   }
   SECTION("scale aabb")
   {
      const auto input= std::vector<vertex_t>{
         {-10,-50},
         {20, -70},
         {10,10},
      };
      auto result = bounding_rect(input);
      REQUIRE(result.min == vertex_t{-10,-70});
      REQUIRE(result.max == vertex_t{20,10});

      result = result * 2;
      REQUIRE(result.min == vertex_t{-20,-140});
      REQUIRE(result.max == vertex_t{40,20});
   }
   SECTION("center point")
   {
      const auto input = bounding_rect<vertex_t>({
         {0,0},
         {10,20},
      });
      const auto result = input.center();
      REQUIRE(result.y == 5);
      REQUIRE(result.z == 10);
   }
   SECTION("translate bbox")
   {
      auto result = bounding_rect<vertex_t>({
         {0,0},
         {10,20},
      });
      result.translate(vertex_t{5, 5});
      REQUIRE(result.min == vertex_t{5,5});
      REQUIRE(result.max == vertex_t{15,25});
   }
}
