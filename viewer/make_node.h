#pragma once

#include "aabb.h"
#include "handles.h"
#include "material.h"
#include "model_transform.h"
#include "position.h"
#include "selectable.h"
#include "sphere.h"
#include "tags.h"
#include "vertex_buffer_id.h"
#include "viewable.h"

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
   } // namespace detail

   void make_node(entt::registry& registry, float radius, glm::vec3 const& pos)
   {
      const auto bbox = aabb{pos - radius, pos + radius};
      const auto bounding_volume = sphere{pos, radius};

      auto entity = registry.create();
      registry.emplace<node_tag>(entity);
      registry.emplace<selectable>(entity);
      //! \todo assign text
      //! \todo assign bounding box or bounding volume
      registry.emplace<sphere>(entity, bounding_volume);
      registry.emplace<aabb>(entity, bbox);
      registry.emplace<vertex_buffer_id>(entity, handle_vertexbuffer_sphere);
      registry.emplace<position>(entity, pos);
      registry.emplace<model_transformation>(entity, detail::node_transformation(pos, radius));
      registry.emplace<material>(entity, ruby);
      registry.emplace<viewable>(entity, true);
   }

   template <typename T>
   void make_node(entt::registry& registry, float radius, std::array<T, 2> pos)
   {
      make_node(registry, radius, glm::vec3{std::get<0>(pos), std::get<1>(pos), 0.0});
   }

   void make_displaced_node(entt::registry& registry, float radius, glm::vec3 const& pos)
   {
      auto entity = registry.create();
      registry.emplace<node_tag>(entity);
      registry.emplace<displacement_tag>(entity);
      registry.emplace<vertex_buffer_id>(entity, handle_vertexbuffer_sphere);
      registry.emplace<position>(entity, pos);
      registry.emplace<model_transformation>(entity, detail::node_transformation(pos, radius));
      registry.emplace<material>(entity, emerald);
      registry.emplace<viewable>(entity, false);
   }
} // namespace sec21::viewer