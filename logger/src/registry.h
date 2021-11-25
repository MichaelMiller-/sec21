#pragma once

#include <array>
// #include <execution>
#include <memory>
#include <type_traits>

namespace sec21::log
{
   template <typename... Ts>
   class registry
   {
      std::array<std::unique_ptr<base_logger>, sizeof...(Ts)> loggers;

      registry() : loggers{std::make_unique<Ts>()...}
      {
         static_assert(std::conjunction_v<std::is_base_of<base_logger, Ts>...>);
      }
      ~registry() = default;

    public:
      void log(entry const& e)
      {
         //! \todo std::execution::par_unseq need some linker settings=
         std::for_each(begin(loggers), end(loggers), [&](auto&& ptr) { ptr->log(e); });
      };

      [[nodiscard]] static auto& instance()
      {
         static registry result;
         return result;
      }
   };
} // namespace sec21::log