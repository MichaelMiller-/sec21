#pragma once

#include <functional>
#include <tuple>

namespace sec21
{
   template <typename... Ts>
   struct any_of : std::tuple<Ts...>
   {
      using std::tuple<Ts...>::tuple;

      template <typename U>
      constexpr bool operator<(U const& u) const noexcept
      {
         return std::apply([&](auto const&... v) { return ((v < u) || ...); }, get());
      }

      template <typename U>
      constexpr bool operator<=(U const& u) const noexcept
      {
         return std::apply([&](auto const&... v) { return ((v <= u) || ...); }, get());
      }

      template <typename U>
      constexpr bool operator>(U const& u) const noexcept
      {
         return std::apply([&](auto const&... v) { return ((v > u) || ...); }, get());
      }

      template <typename U>
      constexpr bool operator>=(U const& u) const noexcept
      {
         return std::apply([&](auto const&... v) { return ((v > u) || ...); }, get());
      }

      template <typename... Args>
      constexpr bool operator()(Args&&... args) const
      {
         return std::apply([&](auto const&... v) { return (v(args...) || ...); }, get());
      }

    private:
      constexpr auto get() const noexcept -> std::tuple<Ts...> const& { return *this; }
   };
} // namespace sec21
