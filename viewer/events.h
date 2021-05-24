#pragma once

#include <array>
#include <limits>
#include <string>

namespace sec21::viewer
{
   //! \todo namespace instead of nacm decoration
   struct event_quit_application
   {
   };

   struct event_clear_entites
   {
   };

   struct event_load_project
   {
      std::size_t id{std::numeric_limits<std::size_t>::max()};
   };
   struct event_load_file
   {
      std::string filename{};
   };
   //! \todo smilify to load_file(...)
   struct event_load_model
   {
      std::string filename{};
   };
   struct event_load_model_load
   {
      // std::string model{};
      std::string load{};
   };

   struct event_load_debug_data
   {
   };

   struct event_new_node
   {
      using value_t = std::array<double, 3>;
      value_t value{};
   };

   // enum class MouseWheelDirection
   // {
   //    Up,
   //    Down
   // };
   struct event_mouse_wheel
   {
      // MouseWheelDirection direction;
      int value{0};
   };
} // namespace sec21::viewer