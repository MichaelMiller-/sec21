#pragma once

#include "tags.h"
#include "material.h"
#include "handles.h"
#include "model_transform.h"
#include "vertex_buffer_id.h"
#include "position.h"
#include "viewable.h"

#include <entt/entt.hpp>

namespace sec21::viewer
{
   namespace detail
   {
      auto arrow_transformation(float radius, glm::vec3 const& pos, glm::vec3 const& dir) -> glm::mat4
      {
         auto T = glm::translate(glm::mat4{ 1.0f }, pos);
         auto S = glm::scale(glm::mat4{ 1.0f }, glm::vec3{ radius, radius, radius * 2 });
         auto Ry = glm::rotate(glm::mat4{ 1.0f }, glm::radians(90.0f), glm::vec3{ 0, dir.x < 0 ? -1 : 1, 0 });

         auto normalized_direction = glm::normalize(dir);
         auto project_x = decltype(normalized_direction){ normalized_direction.x, 0.f, 0.f };

         const auto m = glm::dot(normalized_direction, project_x);
         const auto l1 = glm::length(normalized_direction);
         const auto l2 = glm::length(project_x);         

         auto cos_phi = 0.f;
         if (m == 0)
            //! todo: glm or std constant
            cos_phi = boost::math::constants::half_pi<decltype(cos_phi)>();
         else
            cos_phi = std::acos(m / (l1 * l2));

         auto k = -1.f;
         if (normalized_direction.y > 0)
            k *= -1;

         auto Rz = glm::rotate(glm::mat4{ 1.0f }, cos_phi, glm::vec3{ 0.f, 0.f, k });

         return T * Rz * Ry * S;
      }
   } // detail
   
   //! \todo argument pos and dir could be any vector3d type
   void make_single_load(entt::registry& registry, float scale, glm::vec3 const& pos, glm::vec3 const& dir)
   {
      //! \todo code duplication in transformation
      auto normalized_direction = glm::normalize(dir);      
      auto p0 = pos - (normalized_direction * 0.5f);
      const auto bounding_volume = sphere{ p0, scale };

      auto entity = registry.create();
      registry.emplace<load_tag>(entity);
      registry.emplace<selectable>(entity);
      //! \todo assign text
      //! \todo assign bounding box or bounding volume
      registry.emplace<sphere>(entity, bounding_volume);
      registry.emplace<aabb>(entity);
      registry.emplace<vertex_buffer_id>(entity, handle_vertexbuffer_arrow);
      registry.emplace<position>(entity, pos);
      registry.emplace<model_transformation>(entity, detail::arrow_transformation(scale, pos, dir));
      registry.emplace<material>(entity, ruby);
      registry.emplace<viewable>(entity, true);
   }

   template <typename T, typename Direction>
   void make_single_load(entt::registry& registry, float scale, std::array<T, 2> const& pos, Direction const& dir)
   {
      make_single_load(registry, scale, glm::vec3{ std::get<0>(pos), std::get<1>(pos), 0.0 }, dir);
   }
}
