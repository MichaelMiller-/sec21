#pragma once

#include <glm/vec4.hpp>
#include <glm/matrix.hpp>

namespace sec21::viewer
{
   struct transformation
   {
      glm::ivec4 viewport{};
      glm::mat4 projection{}; // glm::mat4(1.0f)
      glm::mat4 view{};
   };

   inline auto viewport_width(transformation const& t)
   {
      return t.viewport[2];
   }

   inline auto viewport_height(transformation const& t)
   {
      return t.viewport[3];
   }

   inline auto viewport_ratio(transformation const& t)
   {
      // excepts t.viewport.height == screen_height(t) != 0
      return static_cast<float>(viewport_width(t)) / static_cast<float>(viewport_height(t));
   }
}