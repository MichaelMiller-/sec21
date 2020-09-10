#pragma once

#include <sec21/overloaded.h>
#include <sec21/event/events.h>

#include <string_view>
#include <variant>
#include <chrono>
#include <thread>
#include <vector>

namespace sec21::event
{
   //! \brief generic and replayable input manager 
   template <typename Backend>
   class input_manager : Backend
   {
      using clock_t = std::chrono::steady_clock;
      //! \brief
      clock_t::time_point last_tick{ clock_t::now() };
      //! \brief
      std::vector<event_t>	pending_events{};

   public:      
      template <typename... BackendArgs>
      event_t next_event(BackendArgs &&... args)
      {
         if (not empty(pending_events))
         {
            const auto event = pending_events.front();
            pending_events.erase(pending_events.begin());

            match(event,
               [](std::monostate const&) {},
               [](time_elapsed const& obj) { std::this_thread::sleep_for(obj.elapsed); },
               [](auto const& e) {});

            return event;
         }

         if (auto [event_occurred, event] = Backend::poll_event(std::forward<BackendArgs>(args)...); event_occurred)
            return event;

         const auto next_tick = clock_t::now();
         const auto elapsed_time = next_tick - last_tick;
         last_tick = next_tick;

         return time_elapsed{ elapsed_time };
      }

      //! \brief insert events at the top of the queue
      void insert(std::vector<event_t> events) noexcept
      {
         pending_events = std::move(events);
      }
   };

   namespace detail
   {
      template <typename... Ts>
      void choose_event_type(nlohmann::json const& j, std::variant<std::monostate, Ts...>& events)
      {
         using namespace ::sec21::event;
         bool matched = false;
         auto try_variant_type = [&]<typename T>()
         {
            if (not matched)
            {
               try 
               {
                  T obj;
                  from_json(j, obj);
                  events = obj;
                  matched = true;
               }
               catch (std::exception const& ex)
               {
                  // ignore parse error and try the next type in the variant
               }
            }
         };
         (try_variant_type.template operator()<Ts>(), ...);
      }
   }

   template <typename T> requires is_event_type_v<T>
   void to_json(nlohmann::json& j, T const& obj)
   {
      j = nlohmann::json{ { obj.name, { { obj.source.name, obj.source } } } };
   }

   template <typename T> requires is_event_type_v<T>
   void from_json(nlohmann::json const& j, T& obj)
   {
      j.at(obj.name.data()).get_to(obj.source);
   }

   void to_json(nlohmann::json& j, event_t const& event)
   {
      match(event,
         [](std::monostate const&) {},
         [&j](auto const& e) { to_json(j, e); });
   }
   
   void from_json(nlohmann::json const& j, event_t& event)
   {
      detail::choose_event_type(j, event);
   }
}
