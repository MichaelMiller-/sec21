#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include "../intersection.h"

#include <boost/math/constants/constants.hpp>
using namespace boost::math::constants;

// #pragma warning(push)
// #pragma warning(disable:4201)
#include <glm/gtx/string_cast.hpp>
// #pragma warning(pop)

// needed for catch
namespace sec21::viewer
{
   bool operator == (plane const& lhs, plane const& rhs)
   {
      return 
         lhs.a == Approx(rhs.a) &&
         lhs.b == Approx(rhs.b) &&
         lhs.c == Approx(rhs.c) &&
         lhs.d == Approx(rhs.d);
   }

   template <typename CharT, typename Traits, auto N, typename T, glm::qualifier Q>
   std::basic_ostream<CharT, Traits>& operator << (std::basic_ostream<CharT, Traits>& os, glm::vec<N, T, Q> const& v)
   {
      return os << glm::to_string(v);
   }
} // sec21::viewer

//! \todo possible to implement a as library
TEST_CASE("sphere intersects sphere", "[sec21][viewer]")
{
   using namespace sec21::viewer;

   SECTION("hit sphere")
   {
      auto sphere1 = sphere{ { 0.0, 0.0, 0.0 }, 1.0 };
      auto sphere2 = sphere{ { 1.0, 1.0, 0.0 }, 1.0 };
      REQUIRE(intersect(sphere1, sphere2) == true);
   }
   SECTION("sphere touches each other")
   {
      auto sphere1 = sphere{ { 0.0, 0.0, 0.0 }, 1.0 };
      auto sphere2 = sphere{ { 0.0, 1.0, 0.0 }, 1.0 };
      REQUIRE(intersect(sphere1, sphere2) == true);
   }
   SECTION("one sphere contains another")
   {
      auto sphere1 = sphere{ { 0.0, 0.0, 0.0 }, 3.0 };
      auto sphere2 = sphere{ { 1.0, 1.0, 0.0 }, 1.0 };
      REQUIRE(intersect(sphere1, sphere2) == true);
   }
   SECTION("no intersection")
   {
      auto sphere1 = sphere{ { 0.0, 0.0, 0.0 }, 1.0 };
      auto sphere2 = sphere{ { 3.0, 3.0, 0.0 }, 1.0 };
      REQUIRE(intersect(sphere1, sphere2) == false);
   }
}
TEST_CASE("ray intersects circle (basically a sphere with a z-coordinate from zero)", "[sec21][viewer]")
{
   using namespace sec21::viewer;

   auto ray_from_origin = ray{ { 0.0, 0.0, 0.0 }, glm::normalize(glm::vec3{ 1.0, 1.0, 0.0 }) };

   SECTION("unit circle")
   {
      REQUIRE(intersect(ray_from_origin, sphere{ { 3.0, 3.0, 0.0 }, 1.0f }));
   }
   // SECTION("tangent a circle")
   // {
   //    const auto tangent_circle = sphere{ { 1.0, 0.0, 0.0 }, 1.2f - boost::math::constants::one_div_root_two<float>() };
   //    REQUIRE(intersect(ray_from_origin, tangent_circle));
   // }   
   SECTION("invalid circles")
   {
      REQUIRE(intersect(ray_from_origin, sphere{ { 0.0, 3.0, 0.0 }, 1.0f }) == false);
      REQUIRE(intersect(ray_from_origin, sphere{ { 3.0, 0.0, 0.0 }, 1.0f }) == false);

      const auto close_circle = sphere{ { 1.0, 0.0, 0.0 }, 0.9f - one_div_root_two<float>() };
      REQUIRE(intersect(ray_from_origin, close_circle) == false);
   }   
}
TEST_CASE("ray intersect spheres", "[sec21][viewer]")
{
   using namespace sec21::viewer;

   auto ray_from_origin = ray{ { 0.0, 0.0, 0.0 }, glm::normalize(glm::vec3{ 1.0, 1.0, 1.0 }) };

   SECTION("unit sphere")
   {
      REQUIRE(intersect(ray_from_origin, sphere{ { 3.0, 3.0, 3.0 }, 1.0f }));
   }
   SECTION("invalid spheres")
   {
      REQUIRE(intersect(ray_from_origin, sphere{ { 0.0, 3.0, 3.0 }, 1.0f }) == false);
      REQUIRE(intersect(ray_from_origin, sphere{ { 3.0, 0.0, 3.0 }, 1.0f }) == false);
      REQUIRE(intersect(ray_from_origin, sphere{ { 3.0, 3.0, 0.0 }, 1.0f }) == false);
   }
}
TEST_CASE("test plane implementation", "[sec21][detail][viewer]")
{
   using namespace sec21::viewer;

   SECTION("create plane in positive x from point and normale")
   {
      auto pln = plane::from_point_and_normal(glm::vec3{2.0, 0.0, 0.0}, glm::vec3{ 1.0, 0.0, 0.0 });

      REQUIRE(pln.a == Approx(1.0));
      REQUIRE(pln.b == Approx(0.0));
      REQUIRE(pln.c == Approx(0.0));
      REQUIRE(pln.d == Approx(2.0));
   }
   SECTION("create plane in negative x from point and normale")
   {
      auto pln = plane::from_point_and_normal(glm::vec3{2.0, 0.0, 0.0}, glm::vec3{ -1.0, 0.0, 0.0 });

      REQUIRE(pln.a == Approx(-1.0));
      REQUIRE(pln.b == Approx(0.0));
      REQUIRE(pln.c == Approx(0.0));
      REQUIRE(pln.d == Approx(-2.0));
   }   
   SECTION("create xyz plane from point and normale")
   {
      auto pln = plane::from_point_and_normal(glm::vec3{1.0, 1.0, 1.0}, glm::vec3{ 1.0, 1.0, 1.0 });

      REQUIRE(pln.a == Approx(0.57735));
      REQUIRE(pln.b == Approx(0.57735));
      REQUIRE(pln.c == Approx(0.57735));
      REQUIRE(pln.d == Approx(root_three<decltype(pln.d)>()));
   }
   SECTION("create plane in x axis from a triangle")
   {
      auto pln = plane::from_points(
         glm::vec3{ 2.0, 0.0, 0.0 }, 
         glm::vec3{ 2.0, 2.0, 0.0 }, 
         glm::vec3{ 2.0, 0.0, 2.0 });

      REQUIRE(pln.a == Approx(1.0));
      REQUIRE(pln.b == Approx(0.0));
      REQUIRE(pln.c == Approx(0.0));
      REQUIRE(pln.d == Approx(2.0));
   }
   SECTION("create plane from points perpendicular to xy")
   {
      auto pln = plane::from_points(
         glm::vec3{ 2.0, 0.0, 0.0 }, 
         glm::vec3{ 0.0, 2.0, 0.0 }, 
         glm::vec3{ 2.0, 0.0, 2.0 });

      REQUIRE(pln.a == Approx(0.707107));
      REQUIRE(pln.b == Approx(0.707107));
      REQUIRE(pln.c == Approx(0.0));
      REQUIRE(pln.d == Approx(root_two<decltype(pln.d)>()));
   }   
   SECTION("create plane from points")
   {
      auto pln = plane::from_points(
         glm::vec3{ -5.0, -1.0, 7.0 }, 
         glm::vec3{  1.0,  2.0, 8.0 }, 
         glm::vec3{  7.0, -1.0, 9.0 });

      REQUIRE(pln.a == Approx(0.1644));
      REQUIRE(pln.b == Approx(0.0));
      REQUIRE(pln.c == Approx(-0.98639));
      REQUIRE(pln.d == Approx(-7.72675));
   }
   SECTION("distance from different points to a plane")
   {
      const auto pln = plane::from_point_and_normal(glm::vec3{2.0, 0.0, 0.0}, glm::vec3{ 1.0, 0.0, 0.0 });

      REQUIRE(signed_distance(pln, glm::vec3{ 0.0, 0.0, 0.0 }) == Approx(-2.0));
      REQUIRE(signed_distance(pln, glm::vec3{ -2.0, 0.0, 0.0 }) == Approx(-4.0));
      REQUIRE(signed_distance(pln, glm::vec3{ 2.0, 0.0, 2.0 }) == Approx(0.0));
      REQUIRE(signed_distance(pln, glm::vec3{ 4.0, 0.0, 2.0 }) == Approx(2.0));
   }
   SECTION("classify different points")
   {
      auto pln = plane::from_point_and_normal(glm::vec3{-1.0, 0.0, 0.0}, glm::vec3{ -1.0, 0.0, 0.0 });

      REQUIRE(classify(pln, glm::vec3{  0.0, 0.0, 0.0 }) == Face::Back);
      REQUIRE(classify(pln, glm::vec3{  1.5, 1.5, 1.5 }) == Face::Back);


   }
   SECTION("classify different points with a plane created from a position and a normale")
   {
      auto pln = plane::from_point_and_normal(glm::vec3{2.0, 0.0, 0.0}, glm::vec3{ 1.0, 1.0, 0.0 });

      REQUIRE(classify(pln, glm::vec3{  0.0, 0.0, 0.0 }) == Face::Back);
      REQUIRE(classify(pln, glm::vec3{  3.0, 3.0, 3.0 }) == Face::Front);
      REQUIRE(classify(pln, glm::vec3{  2.1, 0.0, 0.0 }) == Face::Front);
      REQUIRE(classify(pln, glm::vec3{  0.0, 3.0, 3.0 }) == Face::Front);
      REQUIRE(classify(pln, glm::vec3{  2.0,-3.0,-3.0 }) == Face::Back);
      REQUIRE(classify(pln, glm::vec3{ -1.0, 3.0,-3.0 }) == Face::Back);
      REQUIRE(classify(pln, glm::vec3{ -0.1, 0.0, 0.0 }) == Face::Back);
   }
   SECTION("classify same points with a plane created by a triangle perpendicular to xy")
   {
      auto pln = plane::from_points(
         glm::vec3{ 2.0, 0.0, 0.0 }, 
         glm::vec3{ 0.0, 2.0, 0.0 }, 
         glm::vec3{ 2.0, 0.0, 2.0 });

      REQUIRE(classify(pln, glm::vec3{  0.0, 0.0, 0.0 }) == Face::Back);
      REQUIRE(classify(pln, glm::vec3{  3.0, 3.0, 3.0 }) == Face::Front);
      REQUIRE(classify(pln, glm::vec3{  2.1, 0.0, 0.0 }) == Face::Front);
      REQUIRE(classify(pln, glm::vec3{  0.0, 3.0, 3.0 }) == Face::Front);
      REQUIRE(classify(pln, glm::vec3{  2.0,-3.0,-3.0 }) == Face::Back);
      REQUIRE(classify(pln, glm::vec3{ -1.0, 3.0,-3.0 }) == Face::Back);
      REQUIRE(classify(pln, glm::vec3{ -0.1, 0.0, 0.0 }) == Face::Back);
   }
#if 0   
   SECTION("extract planes from a axis aligned bounding box")
   {
      auto result = planes_face_outside(aabb{ { 1.0, 1.0, 0.0 }, { 3.0, 8.0, 1.0 } });
      //! \todo validate!!!
      REQUIRE(result[0] == plane{ -1.0, 0.0, 0.0, -1.0 });
      REQUIRE(result[1] == plane{  0.0,-1.0, 0.0, -1.0 });
      REQUIRE(result[2] == plane{  0.0, 0.0,-1.0, 0.0 });
      REQUIRE(result[3] == plane{  1.0, 0.0, 0.0, 3.0 });
      REQUIRE(result[4] == plane{  0.0, 1.0, 0.0, 8.0 });
      REQUIRE(result[5] == plane{  0.0, 0.0, 1.0, 1.0 });
   }
   SECTION("check if points are inside a axis aligned bounding box")
   {
      auto bbox = aabb{ { 1.0, 1.0, 1.0 }, { 3.0, 8.0, 2.0 } };

      REQUIRE(is_inside(bbox, glm::vec3{ 1.1, 1.1, 1.1 }) == true);
      REQUIRE(is_inside(bbox, glm::vec3{ 1.5, 1.5, 1.5 }) == true);
      REQUIRE(is_inside(bbox, glm::vec3{ 2.0, 4.0, 1.5 }) == true);
      REQUIRE(is_inside(bbox, glm::vec3{ 2.5, 1.9, 1.5 }) == true);
      REQUIRE(is_inside(bbox, glm::vec3{ 1.5, 2.0, 1.1 }) == true);
      REQUIRE(is_inside(bbox, glm::vec3{ 1.0, 1.0, 1.0 }) == false);
      REQUIRE(is_inside(bbox, glm::vec3{ -1.0,  0.0, 0.0 }) == false);
      REQUIRE(is_inside(bbox, glm::vec3{  0.0,  0.0, 0.0 }) == false);
      REQUIRE(is_inside(bbox, glm::vec3{  0.5,  0.9, 0.0 }) == false);
      REQUIRE(is_inside(bbox, glm::vec3{  0.5,  0.9, 0.0 }) == false);
   }
   SECTION("check if points are inside a unit box")
   {   
      auto unit_box = aabb{ { -1.0, -1.0, -1.0 }, { 1.0, 1.0, 1.0 } };

      REQUIRE(is_inside(unit_box, glm::vec3{ 0.0, 0.0, 0.0 }) == true);
      REQUIRE(is_inside(unit_box, glm::vec3{ 0.5, 0.3, 0.0 }) == true);
      REQUIRE(is_inside(unit_box, glm::vec3{ 0.0, 1.5, 0.0 }) == false);
      REQUIRE(is_inside(unit_box, glm::vec3{ 4.0, 2.0, 3.0 }) == false);
   }
#endif   
}
TEST_CASE("ray intersect with different boxes", "[sec21][viewer]")
{
   using namespace sec21::viewer;

   SECTION("inside the box")
   {
      auto ray_from_origin = ray{ { 0.0, 0.0, 0.0 }, { 1.0, 0.0, 0.0 } };
      auto bbox = aabb{ { -1.0, -1.0, -1.0 }, { 1.0, 1.0, 1.0 } };
      REQUIRE(intersect(ray_from_origin, bbox));
   }
   SECTION("hit the box")
   {
      auto ray_from_origin = ray{ { 0.0, 0.0, 0.0 }, glm::normalize(glm::vec3{ 1.0, 1.0, 1.0 }) };
      auto bbox = aabb{ { 2.0, 2.0, 2.0 }, { 4.0, 4.0, 4.0 } };
      REQUIRE(intersect(ray_from_origin, bbox) == true);
   }
   SECTION("ray misses box")
   {
      auto ray_in_x = ray{ { 0.0, 0.0, 0.0 }, { 1.0, 0.0, 0.0 } };
      auto bbox = aabb{ { 2.0, 2.0, 2.0 }, { 4.0, 4.0, 4.0 } };
      REQUIRE(intersect(ray_in_x, bbox) == false);
   }
   SECTION("invalid ray misses box")
   {
      auto invalid_ray = ray{ { 0.0, 0.0, 0.0 }, { 1.0, 0.0, 1.0 } };
      auto bbox = aabb{ { 2.0, 2.0, 2.0 }, { 4.0, 4.0, 4.0 } };
      REQUIRE(intersect(invalid_ray, bbox) == false);
   }
}
