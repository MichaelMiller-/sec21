#pragma once

#include <string>
#include <array>

namespace sec21::viewer
{
   //! \todo namespace instead of nacm decoration
   struct event_quit_application {};

   struct event_clear_entites {};

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

   struct event_load_debug_data {};

   struct event_new_node
   {
      using value_t = std::array<double, 3>;
      value_t value{};
   }; 

   /// Input Events
   ///
   struct event_mouse_move
   {
      using value_t = std::array<int, 2>;
      value_t position{};
   };


   namespace event
   {
      struct mouse_button_down
      {
         std::array<bool, 3> pressed{};
         std::array<int, 2> relative_to_window{};
         int clicks{};
      };
      struct mouse_button_up
      {
         std::array<bool, 3> released{};
         std::array<int, 2> relative_to_window{};
         int clicks{};
      };
      struct mouse_move
      {
         std::array<bool, 3> button_pressed{};
         std::array<int, 2> relative_to_window{};
         std::array<int, 2> relative_motion{};
      };
   }

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

   struct event_key_down
   {
      std::int32_t value{0};
   };
}