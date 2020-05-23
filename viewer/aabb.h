#pragma once

#include <glm/vec3.hpp>

namespace sec21::viewer
{
   struct aabb
   {
      glm::vec3 min{};
      glm::vec3 max{};
   };

   // template <typename BoundingVolume, typename Point>
   // void add_point(BoundingVolume& bounding_volume, Point const& pt)
   // {
   // }
}