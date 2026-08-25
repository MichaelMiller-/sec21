#pragma once

#include <functional>
#include <tuple>

namespace sec21
{
   template <typename... Ts>
   class any_of
   {
      std::tuple<Ts...> m_values;

   public:
      constexpr explicit any_of(Ts... values) : m_values{std::move(values)...} {}

      template <typename U>
      constexpr bool operator<(U const& u) const noexcept
      {
         return std::apply([&](auto const&... v) { return ((v < u) || ...); }, m_values);
      }

      template <typename U>
      constexpr bool operator<=(U const& u) const noexcept
      {
         return std::apply([&](auto const&... v) { return ((v <= u) || ...); }, m_values);
      }

      template <typename U>
      constexpr bool operator>(U const& u) const noexcept
      {
         return std::apply([&](auto const&... v) { return ((v > u) || ...); }, m_values);
      }

      template <typename U>
      constexpr bool operator>=(U const& u) const noexcept
      {
         return std::apply([&](auto const&... v) { return ((v > u) || ...); }, m_values);
      }

      template <typename... Args>
      constexpr bool operator()(Args&&... args) const
      {
         return std::apply([&](auto const&... v) { return (v(args...) || ...); }, m_values);
      }
   };

   template <typename... Ts>
   any_of(Ts...) -> any_of<Ts...>;

} // namespace sec21
