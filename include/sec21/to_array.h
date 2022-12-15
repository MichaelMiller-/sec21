#pragma once

#include <functional>
#include <tuple>
#include <array>

namespace sec21
{
   namespace detail
   {
      template <typename... Ts, typename Callable, std::size_t... Is>
      constexpr auto to_array_impl(std::tuple<Ts...> const& input, Callable&& f, std::index_sequence<Is...>)
      {
         return std::array{ f(std::get<Is>(input))... };
      }
   }

   //! \brief transform a std::tuple<Ts...> into a std::array<U, tuple_size>
   template <typename... Ts, typename Callable>
   [[nodiscard]] constexpr auto to_array(std::tuple<Ts...> const& input, Callable&& f)
   {
#if __cpp_generic_lambdas >= 201707L
      return [&]<size_t... Is>(std::index_sequence<Is...>) noexcept(
         noexcept(std::is_nothrow_invocable_v<Callable, Ts...>))
      {
         return std::array{ f(std::get<Is>(input))... };
      }(std::make_index_sequence<sizeof...(Ts)>{});
#else
      return detail::to_array_impl(input, std::forward<Callable>(f), std::make_index_sequence<sizeof...(Ts)>{});
#endif
   }
}
