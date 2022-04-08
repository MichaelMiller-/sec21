#include <catch.hpp>

#include <sec21/polygon.h>
#include <sec21/structural_analysis/shape/cross_section_vertex.h>

// rect
const auto test_polygon = R"({
"vertices": [
   {
      "y": -20,
      "z": -30
   },
   {
      "y": 0,
      "z": -30
   },
   {
      "y": 0,
      "z": 70
   },
   {
      "y": -20,
      "z": 70
   }
]
})"_json;

TEST_CASE("polygon class", "[sec21]")
{
   using namespace sec21;

   using vertex_t = structural_analysis::shape::cross_section_vertex<int>;
   const auto input = test_polygon.get<polygon<vertex_t>>();

   REQUIRE(size(input.vertices) == 4);

   SECTION("extent")
   {
      REQUIRE(input.extent() == 240);
   }
   SECTION("area")
   {
      REQUIRE(input.area() == 2000);
   }
   SECTION("center")
   {
      const auto result = input.center();
      REQUIRE(result.y == -10);
      REQUIRE(result.z == 20);
   }
   SECTION("move polygon")
   {
      auto cp = input;
      cp.translate(vertex_t{10, -20});
      const auto result = cp.center();
      REQUIRE(result.y == 0);
      REQUIRE(result.z == 0);
   }
   SECTION("convert to triangle list")
   {
      const auto result = to_triangle_list(input);
      const auto expected = std::array<vertex_t, 6>{
         vertex_t{-20,-30},
         { 0, -30},
         { 0,70},
         {-20,-30},
         {0,70},
         {-20,70},
      };
      REQUIRE(size(result) == size(expected));
      REQUIRE(std::equal(begin(result), end(result), std::begin(expected)));
   }
}
