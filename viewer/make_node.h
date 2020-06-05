#pragma once

#include "tags.h"
#include "material.h"
#include "handles.h"
#include "model_transform.h"
#include "vertex_buffer_id.h"
#include "position.h"
#include "viewable.h"
#include "selectable.h"
#include "aabb.h"
#include "sphere.h"

#include <entt/entt.hpp>

namespace sec21::viewer
{
   namespace detail
   {
      auto node_transformation(glm::vec3 const& pos, float radius) -> glm::mat4
      {
         auto result = glm::mat4(1.0f);
         result = glm::translate(result, pos);
         result = glm::scale(result, glm::vec3(radius, radius, radius));
         return result;
      }
   } // detail
   
   void make_node(entt::registry& registry, float radius, glm::vec3 const& pos)
   {
      const auto bbox = aabb{ pos - radius, pos + radius };
      const auto bounding_volume = sphere{ pos, radius };

      auto entity = registry.create();
      registry.assign<node_tag>(entity);
      registry.assign<selectable>(entity);
      //! \todo assign text
      //! \todo assign bounding box or bounding volume
      registry.assign<sphere>(entity, bounding_volume);
      registry.assign<aabb>(entity, bbox);
      registry.assign<vertex_buffer_id>(entity, handle_vertexbuffer_sphere);
      registry.assign<position>(entity, pos);
      registry.assign<model_transformation>(entity, detail::node_transformation(pos, radius));
      registry.assign<material>(entity, ruby);
      registry.assign<viewable>(entity, true);
   }

   template <typename T>
   void make_node(entt::registry& registry, float radius, std::array<T, 2> const& pos)
   {
      make_node(registry, radius, glm::vec3{ std::get<0>(pos), std::get<1>(pos), 0.0 });
   }   

   void make_displaced_node(entt::registry& registry, float radius, glm::vec3 const& pos)
   {
      auto entity = registry.create();
      registry.assign<node_tag>(entity);
      registry.assign<displacement_tag>(entity);
      registry.assign<vertex_buffer_id>(entity, handle_vertexbuffer_sphere);
      registry.assign<position>(entity, pos);
      registry.assign<model_transformation>(entity, detail::node_transformation(pos, radius));
      registry.assign<material>(entity, emerald);
      registry.assign<viewable>(entity, false);	
   }
}