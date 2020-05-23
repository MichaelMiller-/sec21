#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace sec21::viewer
{
   //! \todo maybe rename to input_state
   struct input_data
   {
      //! \todo should hold a state of the current input
      bool mouse_btn_left_clicked{ false };
      bool mouse_btn_right_clicked{ false };
      glm::ivec2 mouse_position{};
      glm::ivec3 mouse_3d_start{};
      glm::ivec3 mouse_3d_end{};
   };
}