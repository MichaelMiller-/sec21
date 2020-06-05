#pragma once

#include "tags.h"
#include "material.h"
#include "handles.h"
#include "model_transform.h"
#include "vertex_buffer_id.h"
#include "position.h"
#include "viewable.h"

#include <entt/entt.hpp>

#include <boost/math/constants/constants.hpp>

namespace sec21::viewer
{
   namespace detail
   {
      auto fixed_support_transformation(glm::vec3 const& pos, float scale) -> glm::mat4
      {
         auto result = glm::mat4(1.0f);
         result = glm::translate(result, pos);
         result = glm::scale(result, glm::vec3(scale));
         //! \todo glm::half_pi?
         result = glm::rotate(result, boost::math::constants::half_pi<float>(), glm::vec3(-1.0f, 0.0f, 0.0f));
         return result;
      }
   } // detail
   
   void make_fixed_support(entt::registry& registry, float radius, glm::vec3 const& pos)
   {
      auto entity = registry.create();
      registry.assign<support_tag>(entity);
      registry.assign<selectable>(entity);
      registry.assign<vertex_buffer_id>(entity, handle_vertexbuffer_support_fixed);
      registry.assign<position>(entity, pos);
      registry.assign<model_transformation>(entity, detail::fixed_support_transformation(pos, radius));
      registry.assign<material>(entity, ruby);
      registry.assign<viewable>(entity, true);
   }

   template <typename T>
   void make_fixed_support(entt::registry& registry, float radius, std::array<T, 2> const& pos)
   {
      make_fixed_support(registry, radius, glm::vec3{ std::get<0>(pos), std::get<1>(pos), 0.0 });
   }

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
}