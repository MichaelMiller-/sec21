#pragma once

#include <tuple>

namespace sec21
{
   //! \brief transform a std::tuple<Ts...> into a std::tuple<U>
   template <typename... Ts, typename UnaryOperation>
   [[nodiscard]] constexpr auto transform(std::tuple<Ts...> const& input, UnaryOperation unary_op)
      noexcept(noexcept(std::is_nothrow_invocable_v<UnaryOperation, Ts...>))
   {
      return std::apply([&](auto const&... value) { return std::make_tuple(unary_op(value)...); }, input);
   }
} // namespace sec21
