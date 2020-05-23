#pragma once

#include "tags.h"
#include "material.h"
#include "handles.h"
#include "model_transform.h"
#include "vertex_buffer_id.h"
#include "position.h"
#include "viewable.h"

#include <boost/math/constants/constants.hpp>

#include <entt/entt.hpp>

namespace sec21::viewer
{
   namespace detail
   {
      auto axis_transformation(float scale, glm::vec3 const& origin, glm::vec3 const& dir) -> glm::mat4
      {
         auto pp = dir * 0.8f; //! \todo scale is not considered
         auto T = glm::translate(glm::mat4{ 1.0f }, origin + pp);
         auto S = glm::scale(glm::mat4{ 1.0f }, glm::vec3{ scale, scale, scale * 2 });
         auto Ry = glm::rotate(glm::mat4{ 1.0f }, glm::radians(90.0f), glm::vec3{ 0, dir.x < 0 ? -1 : 1, 0 });

         auto normalized_direction = glm::normalize(dir);
         auto project_x = decltype(normalized_direction){ normalized_direction.x, 0.f, 0.f };

         const auto m = glm::dot(normalized_direction, project_x);
         const auto l1 = glm::length(normalized_direction);
         const auto l2 = glm::length(project_x);         

         auto cos_phi = 0.f;
         if (m == 0)
            cos_phi = boost::math::constants::half_pi<decltype(cos_phi)>();
         else
            cos_phi = std::acos(m / (l1 * l2));

         auto k = -1.f;
         if (normalized_direction.y > 0)
            k *= -1;

         auto Rz = glm::rotate(glm::mat4{ 1.0f }, cos_phi, glm::vec3{ 0.f, 0.f, k });

         return T * Rz * Ry * S;
      }
   }

   void make_axis(entt::registry& registry) 
   //! \todo glm::vec3 origin = { 0.0, 0.0, 0.0 }
   //! \todo scale = systemaxis vs local axis 
   {
      {
         auto entity = registry.create();
         registry.assign<axis_tag>(entity);
         registry.assign<vertex_buffer_id>(entity, handle_vertexbuffer_arrow);
         registry.assign<position>(entity, glm::vec3{ 0.0, 0.0, 0.0, });
         registry.assign<model_transformation>(entity, detail::axis_transformation(0.1f, { 0.0, 0.0, 0.0, }, { 1.0, 0.0, 0.0 }));
         registry.assign<material>(entity, ruby);
         registry.assign<viewable>(entity, true);
      }
      {
         auto entity = registry.create();
         registry.assign<axis_tag>(entity);
         registry.assign<vertex_buffer_id>(entity, handle_vertexbuffer_arrow);
         registry.assign<position>(entity, glm::vec3{ 0.0, 0.0, 0.0, });
         registry.assign<model_transformation>(entity, detail::axis_transformation(0.1f, { 0.0, 0.0, 0.0, }, { 0.0, 1.0, 0.0 }));
         registry.assign<material>(entity, emerald);
         registry.assign<viewable>(entity, true);
      }
      //! \todo z-axis
      // {
      //    auto entity = registry.create();
      //    registry.assign<axis_tag>(entity);
      //    registry.assign<vertex_buffer_id>(entity, handle_vertexbuffer_arrow);
      //    registry.assign<position>(entity, glm::vec3{ 0.0, 0.0, 0.0, });
      //    registry.assign<model_transformation>(entity, detail::axis_transformation(0.1f, { 0.0, 0.0, 0.0, }, { 0.0, 0.0, 1.0 }));
      //    registry.assign<material>(entity, turquoise);
      //    registry.assign<viewable>(entity, true);
      // }
   }

}
