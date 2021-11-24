#pragma once

#include <utility>
#include <numeric>
#include <string_view>

namespace sec21::debug_timer
{
   struct timer
   {
      std::string name{};
      double elapsed_seconds{};
      std::size_t counter{};

      auto operator+=(timer const& rhs)
      {
         elapsed_seconds += rhs.elapsed_seconds;
         counter++;
         return *this;
      }
      auto operator+(timer const& rhs) { return timer{name, elapsed_seconds + rhs.elapsed_seconds, counter++}; }
   };

   struct messurement
   {
      std::string name;
      std::vector<timer> timers;

      [[nodiscard]] auto total() const noexcept { return std::accumulate(begin(timers), end(timers), timer{}); }
   };
   namespace detail
   {
      class scoped_timer;

      class registry
      {
         friend scoped_timer;

         messurement result;
         bool locked{false};

         void add_scope_values(std::string_view name, double elapsed_seconds)
         {
            if (auto it =
                   std::find_if(begin(result.timers), end(result.timers), [=](auto t) { return t.name == name; });
                it != end(result.timers)) {
               (*it) += timer{std::string{name}, elapsed_seconds};
            } else {
               result.timers.push_back(timer{std::string{name}, elapsed_seconds, 1});
            }
         }

       public:
         void set_name(std::string_view value)
         {
            // excepts: value is not empty
            result.name = value;
         }

         void lock_scope() { locked = true; }
         void unlock_scope() { locked = false; }
         bool is_locked() const noexcept { return locked; }
         bool has_timer() const noexcept { return size(result.timers) > 0; }

         auto get_result() const noexcept
         {
            //            if (detail::registry::instance().has_timer())
            //               throw std::runtime_error("no scope timer was set");

            return result;
         }
         static auto& instance()
         {
            static registry result;
            return result;
         }
      };

      class scoped_timer
      {
         const std::chrono::time_point<std::chrono::system_clock> begin{std::chrono::system_clock::now()};
         std::string_view scope_name;

       public:
         explicit scoped_timer(std::string_view name) : scope_name{name} { registry::instance().lock_scope(); }
         ~scoped_timer()
         {
            const std::chrono::duration<double> elapsed = std::chrono::system_clock::now() - begin;
            registry::instance().add_scope_values(scope_name, elapsed.count());
            registry::instance().unlock_scope();
         }
      };
   } // namespace detail

   inline void start(std::string_view name) { detail::registry::instance().set_name(std::move(name)); }

   //! important: value cannot be ignored
   [[nodiscard]] inline auto scope(std::string_view name)
   {
      if (detail::registry::instance().is_locked())
         throw std::runtime_error("a debug timer is already running");
      return detail::scoped_timer{name};
   }

   template <typename Callable>
   [[nodiscard]] inline void stop(Callable func)
   {
      func(detail::registry::instance().get_result());
   }
#if 0
   inline void start(std::string_view) {}

   inline auto scope(std::string_view) {}

   template <typename Callable>
   inline void stop(Callable)
   {
   }
#endif
#if 0
   auto string_logger(messurement const& m)
   {
      const auto total = m.total();
      std::vector<std::string> lines;
      std::transform(begin(m.timers), end(m.timers), std::back_inserter(lines), [=](auto e) {
        return fmt::format("{:<20} elapsed: {:f}s total %: {:f}", e.name, e.elapsed_seconds,
                           (e.elapsed_seconds / total.elapsed_seconds) * 100);
      });
      lines.push_back(fmt::format("{:<19} in total: {:f}s", m.name, total.elapsed_seconds));
   }
   auto console_logger(messurement const& m)
   {
      const auto total = m.total();
      for (auto e : m.timers) {
         std::cout << fmt::format("{:<20} elapsed: {:f}s total %: {:f}", e.name, e.elapsed_seconds,
                                  (e.elapsed_seconds / total.elapsed_seconds) * 100)
                   << std::endl;
      }
      std::cout << fmt::format("{:<19} in total: {:f}s", m.name, total.elapsed_seconds) << std::endl;
   }
#endif
} // namespace sec21::debug_timer

#ifndef NDEBUG
#include <nlohmann/json.hpp>

namespace sec21::debug_timer
{
   inline void to_json(nlohmann::json& j, timer const& obj)
   {
      j = nlohmann::json{{"name", obj.name}, {"elapsed_seconds", obj.elapsed_seconds}, {"counter", obj.counter}};
   }
   inline void from_json(nlohmann::json const& j, timer& obj)
   {
      j.at("name").get_to(obj.name);
      j.at("elapsed_seconds").get_to(obj.elapsed_seconds);
      j.at("counter").get_to(obj.counter);
   }
   inline void to_json(nlohmann::json& j, messurement const& obj)
   {
      j = nlohmann::json{{"name", obj.name}, {"timers", obj.timers}};
   }
   inline void from_json(nlohmann::json const& j, messurement& obj)
   {
      j.at("name").get_to(obj.name);
      j.at("timers").get_to(obj.timers);
   }
} // namespace debug_timer
#endif