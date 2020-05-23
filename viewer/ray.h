#pragma once

#include <glm/vec3.hpp>

namespace sec21::viewer
{
   struct ray
   {
      using value_t = glm::vec3;

      value_t origin{};
      value_t dir{}; // normalized vector
   };
}