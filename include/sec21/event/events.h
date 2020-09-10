#pragma once

#include <sec21/event/event_type.h>
#include <sec21/event/close_window.h>
#include <sec21/event/time_elapsed.h>
#include <sec21/event/mouse.h>
#include <sec21/event/mouse_button.h>
#include <sec21/event/key.h>

#include <variant>

namespace sec21::event
{
   // clang-format off
   using event_t = std::variant<
      std::monostate,
      close_window,
      time_elapsed,
      pressed<mouse_button>,
      released<mouse_button>,
      moved<mouse>,
      pressed<key>, 
      released<key>
   >;
   // clang-format on
}
