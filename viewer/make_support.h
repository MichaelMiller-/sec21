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
      auto fixed_support_transformation(glm::vec3 const& pos, float scale, float rotation) -> glm::mat4
      {
         const auto T = glm::translate(glm::mat4(1.0f), pos);
         const auto Rx = glm::rotate(glm::mat4(1.0f), glm::half_pi<float>(), glm::vec3(-1.0f, 0.0f, 0.0f));
         const auto Ry = glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 1.0f, 0.0f));
         const auto S = glm::scale(glm::mat4(1.0f), glm::vec3(scale));

         return T * Rx * Ry * S;
      }
   } // detail
   
   void make_fixed_support(entt::registry& registry, float radius, glm::vec3 const& pos, float rotation = 0.f)
   {
      auto entity = registry.create();
      registry.emplace<support_tag>(entity);
      registry.emplace<selectable>(entity);
      registry.emplace<vertex_buffer_id>(entity, handle_vertexbuffer_support_fixed);
      registry.emplace<position>(entity, pos);
      registry.emplace<model_transformation>(entity, detail::fixed_support_transformation(pos, radius, rotation));
      registry.emplace<material>(entity, ruby);
      registry.emplace<viewable>(entity, true);
   }

   template <typename T>
   void make_fixed_support(entt::registry& registry, float radius, std::array<T, 2> const& pos, float rotation = 0.f)
   {
      make_fixed_support(registry, radius, glm::vec3{ std::get<0>(pos), std::get<1>(pos), 0.0 }, rotation);
   }

#if 0
   void make_support_roller(entt::registry& registry, float radius, glm::vec3 const& pos)
   {
      auto entity = registry.create();
      registry.assign<support_tag>(entity);
      registry.assign<selectable>(entity);
      registry.assign<vertex_buffer_id>(entity, handle_vertexbuffer_support_roller);
      registry.assign<position>(entity, pos);
      //! \todo transformation
      registry.assign<model_transformation>(entity, detail::fixed_support_transformation(pos, radius));
      registry.assign<material>(entity, ruby);
      registry.assign<viewable>(entity, true);
   }
#endif   
}