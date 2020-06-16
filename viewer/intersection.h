#pragma once 

#include "sphere.h"
#include "ray.h"
#include "aabb.h"

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/norm.hpp>

namespace sec21::viewer
{
   //! \web https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection
   //! \web https://www.opengl-tutorial.org/miscellaneous/clicking-on-objects/picking-with-custom-ray-obb-function/

   inline bool intersect(sphere const& s1, sphere const& s2)
   {
      return glm::length2(s1.position - s2.position) <= (s1.radius + s2.radius) * (s1.radius + s2.radius);
   }

   inline auto closest_point(ray const& r, glm::vec3 const& pt)
   { 
      auto ab_square = glm::dot(r.dir, r.dir); 
      auto AP = pt - r.origin; 
      auto ap_dot_ab = glm::dot(AP, r.dir); 
      // t is a projection param when we project vector AP onto AB 
      auto t = ap_dot_ab / ab_square; 
      // calculate the closest point 
      return r.origin + (r.dir * t);
   }

   struct plane
   {
      using value_t = float;

      // general plane equation: ax + by + cz = d
      //! \todo check if it neccessary that a,b,c has to normalized. now there are
      value_t a{};
      value_t b{};
      value_t c{};
      value_t d{};

      template <typename Point, typename Vector>
      static plane from_point_and_normalized_dir(Point const& pt, Vector const& dir)
      {
         const auto d = glm::dot(pt, dir);
         return { dir.x, dir.y, dir.z, d };
      }

      template <typename Point, typename Vector>
      static plane from_point_and_normal(Point const& pt, Vector const& dir)
      {
         const auto coefficient = glm::normalize(dir);
         const auto d = glm::dot(pt, coefficient);
         return { coefficient.x, coefficient.y, coefficient.z, d };
      }

      template <typename Point>
      static plane from_points(Point const& pt1, Point const& pt2, Point const& pt3)
      {
         const auto coefficient = glm::normalize(glm::cross(pt2 - pt1, pt3 - pt1));
         return { coefficient.x, coefficient.y, coefficient.z, glm::dot(coefficient, pt1) };
      }

      friend std::ostream& operator << (std::ostream& os, plane const& p) 
      {
         return os << "((" << p.a << ", " << p.b << ", " << p.c << ") " << p.d << ")";
      }      
   };

   inline auto signed_distance(plane const& p, glm::vec3 const& pt)
   {
      //! \todo assert(p.a == p.b == p.c != 0)
      return glm::dot(glm::vec3{ p.a, p.b, p.c }, pt) - p.d;
   }

   enum class Face
   {
      Front,
      Back
   };

   template <typename Point>
   auto classify(plane const&p, Point const& pt) noexcept -> Face
   {
      if (signed_distance(p, pt) < 0)
         return Face::Back;
      return Face::Front;
   }
#if 0
      inline auto planes_face_outside(aabb const& bbox) noexcept -> std::array<plane, 6>
      {
         return {
            plane::from_point_and_normalized_dir(bbox.min, glm::vec3{ -1.0, 0.0, 0.0 }),
            plane::from_point_and_normalized_dir(bbox.min, glm::vec3{  0.0,-1.0, 0.0 }),
            plane::from_point_and_normalized_dir(bbox.min, glm::vec3{  0.0, 0.0,-1.0 }),
            plane::from_point_and_normalized_dir(bbox.max, glm::vec3{  1.0, 0.0, 0.0 }),
            plane::from_point_and_normalized_dir(bbox.max, glm::vec3{  0.0, 1.0, 0.0 }),
            plane::from_point_and_normalized_dir(bbox.max, glm::vec3{  0.0, 0.0, 1.0 })
         };
      }

      template <typename Point>
      bool is_inside(aabb const& bbox, Point const& pt) noexcept
      {
         const auto pln = planes_face_outside(bbox);
         const auto n =  std::count_if(
            std::begin(pln), 
            std::end(pln), 
            [&pt](auto const& p) { return classify(p, pt) == Face::Front; });
         return n == 0;
      }
#endif
   inline bool intersect(ray const& r, sphere const& s)
   {
      const auto v = closest_point(r, s.position);
      const auto len = glm::distance(v, s.position);
      return len < s.radius;
   }

   inline bool intersect(ray const& r, aabb const& bbox) // float epsilon
   {
      const auto inv = 1.0f / r.dir;

      const auto tx1 = (bbox.min.x - r.origin.x) * inv.x;
      const auto tx2 = (bbox.max.x - r.origin.x) * inv.x;

      auto tmin = std::min(tx1, tx2);
      auto tmax = std::max(tx1, tx2);

      const auto ty1 = (bbox.min.y - r.origin.y) * inv.y;
      const auto ty2 = (bbox.max.y - r.origin.y) * inv.y;

      tmin = std::max(tmin, std::min(ty1, ty2));
      tmax = std::min(tmax, std::max(ty1, ty2));

      const auto tz1 = (bbox.min.y - r.origin.y) * inv.z;
      const auto tz2 = (bbox.max.y - r.origin.y) * inv.z;

      tmin = std::max(tmin, std::min(tz1, tz2));
      tmax = std::min(tmax, std::max(tz1, tz2));

      // return tmax >= tmin;// tmax >= std::max(0.0f, tmin) && tmin < epsilon;
      return tmax >= tmin;
   }
} // sec21::viewer
