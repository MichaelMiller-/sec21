#pragma once

#include "tags.h"
#include "material.h"
#include "viewable.h"
#include "vertex_buffer_id.h"
#include "model_transform.h"

#include <entt/entt.hpp>

namespace sec21::viewer
{
   namespace detail
   {
      template <typename Position>
      auto member_transformation(Position const& from, Position const& to, float radius) -> glm::mat4
      {
         auto d = to - from;
         auto dn = glm::normalize(d);
         auto member_length = glm::length(d);
         auto p0 = from + (dn * (member_length * 0.5f));

         auto aa = dn;
         auto bb = glm::vec3{0.0f, 1.0f, 0.0f};
         auto cc = std::acos(glm::dot(aa, bb) / (glm::length(aa) * glm::length(bb)));

         if (dn.x < 0.0f)
            cc = glm::pi<float>() - cc;

         cc *= -1;

         auto T = glm::translate(glm::mat4(1.0f), p0);
         auto S = glm::scale(glm::mat4(1.0f), glm::vec3(radius, radius, member_length));
         auto Rx = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
         auto Ry = glm::rotate(glm::mat4(1.0f), cc, glm::vec3(0.0f, 1.0f, 0.0f));
         
         return T * Rx * Ry * S;
      }
   }

   void make_member(entt::registry& registry, float radius, glm::vec3 const& from, glm::vec3 const& to)
   {
      //! \todo merge with stretch_cylinder
      const auto dir = to - from;
      const auto len = glm::length(dir);
      const auto ndir = glm::normalize(dir);
      const auto mid = from + (ndir * len * 0.5f);
      const auto bbox = aabb{ mid - radius, mid + radius };
      const auto bounding_volume = sphere{ mid, 0.3f };

      auto entity = registry.create();
      registry.assign<member_tag>(entity);
      registry.assign<selectable>(entity);

      //! \todo assign text
      //! \todo assign bounding box or bounding volume
      registry.assign<sphere>(entity, bounding_volume);
      registry.assign<aabb>(entity, bbox);

      registry.assign<vertex_buffer_id>(entity, handle_vertexbuffer_cylinder);
      registry.assign<model_transformation>(entity, detail::member_transformation(from, to, radius));
      registry.assign<material>(entity, chrome);
      registry.assign<viewable>(entity, true);
   }

   template <typename CoordinateType, typename CoordinateSystem>
   void make_member(
      entt::registry& registry, 
      float radius, 
      boost::geometry::model::point<CoordinateType, 2, CoordinateSystem> const& from,
      boost::geometry::model::point<CoordinateType, 2, CoordinateSystem> const& to)
   {
      make_member(
         registry, 
         radius, 
         glm::vec3{ boost::geometry::get<0>(from), boost::geometry::get<1>(from), 0.0 },
         glm::vec3{ boost::geometry::get<0>(to), boost::geometry::get<1>(to), 0.0 });
   }

   void make_displaced_member(entt::registry& registry, float radius, glm::vec3 const& from, glm::vec3 const& to)
   {
      auto entity = registry.create();
      registry.assign<member_tag>(entity);
      registry.assign<displacement_tag>(entity);
      registry.assign<vertex_buffer_id>(entity, handle_vertexbuffer_cylinder);
      registry.assign<model_transformation>(entity, detail::member_transformation(from, to, radius));
      registry.assign<material>(entity, jade);
      registry.assign<viewable>(entity, false);
   }

   template <typename CoordinateType, typename CoordinateSystem>
   void make_displaced_member(
      entt::registry& registry, 
      float radius, 
      boost::geometry::model::point<CoordinateType, 2, CoordinateSystem> const& from,
      boost::geometry::model::point<CoordinateType, 2, CoordinateSystem> const& to)
   {
      make_displaced_member(
         registry, 
         radius, 
         glm::vec3{ boost::geometry::get<0>(from), boost::geometry::get<1>(from), 0.0 },
         glm::vec3{ boost::geometry::get<0>(to), boost::geometry::get<1>(to), 0.0 });
   }   
}