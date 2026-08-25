#pragma once

#include <functional>
#include <tuple>

namespace sec21
{
   template <typename... Ts>
   class all_of : std::tuple<Ts...>
   {
      std::tuple<Ts...> m_values;

   public:
      constexpr explicit all_of(Ts... values) : m_values{std::move(values)...} {}

      template <typename U>
      constexpr bool operator<(U const& u) const noexcept
      {
         return std::apply([&](auto const&... v) { return ((v < u) && ...); }, m_values);
      }

      template <typename U>
      constexpr bool operator<=(U const& u) const noexcept
      {
         return std::apply([&](auto const&... v) { return ((v <= u) && ...); }, m_values);
      }

      template <typename U>
      constexpr bool operator>(U const& u) const noexcept
      {
         return std::apply([&](auto const&... v) { return ((v > u) && ...); }, m_values);
      }

      template <typename U>
      constexpr bool operator>=(U const& u) const noexcept
      {
         return std::apply([&](auto const&... v) { return ((v > u) && ...); }, m_values);
      }

      template <typename... Args>
      constexpr bool operator()(Args&&... args) const
      {
         return std::apply([&](auto const&... v) { return (v(args...) && ...); }, m_values);
      }
   };

   template <typename... Ts>
   all_of(Ts...) -> all_of<Ts...>;

} // namespace sec21
