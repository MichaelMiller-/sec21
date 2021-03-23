#pragma once

#include <sec21/fixed_string.h>

#include <string_view>

namespace sec21::event
{
#if __cpp_nontype_template_args >= 201911L

   struct event_type_base {};

   template <typename Source, fixed_string Name>
   struct event_type : public event_type_base
   {
      static constexpr std::string_view name{ Name };
      Source source{};
   };

   template <typename T>
   struct is_event_type : std::is_base_of<event_type_base, T> {};

   template <typename T>
   constexpr auto is_event_type_v = is_event_type<T>::value;

   template <typename T, typename... Args> requires is_event_type_v<T>
   [[nodiscard]] constexpr auto make_event(Args &&... args) noexcept
   {
      T obj{};
      obj.source = { std::forward<Args>(args)... };
      return obj;
   }

   /// Event-types
   ///
   template <typename Source>
   using pressed = event_type<Source, "pressed">;

   template <typename Source>
   using released = event_type<Source, "released">;

   template <typename Source>
   using moved = event_type<Source, "moved">;

#endif   
}
