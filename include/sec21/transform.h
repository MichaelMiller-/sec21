#pragma once

#include <functional>
#include <tuple>

namespace sec21
{
   namespace detail
   {
      template <typename... Ts, typename Callable, size_t... Is>
      constexpr auto transform_impl(std::tuple<Ts...> const& input, Callable&& f, std::index_sequence<Is...>)
      {
         return std::tuple<std::invoke_result_t<Callable, Ts>...>{f(std::get<Is>(input))...};
      }
   }

   //! \brief transform a std::tuple<Ts...> into a std::tuple<U>
   template<typename... Ts, typename Callable>
   [[nodiscard]] constexpr auto transform(std::tuple<Ts...> const& input, Callable&& f)
   {
#if __cpp_generic_lambdas >= 201707L
      return [&]<size_t... Is>(std::index_sequence<Is...>) noexcept(
         noexcept(std::is_nothrow_invocable_v<Callable, Ts...>))
      {
         return std::tuple<std::invoke_result_t<Callable, Ts>...>{ f(std::get<Is>(input))... };
      }(std::make_index_sequence<sizeof...(Ts)>{});
#else
      return detail::transform_impl(input, std::forward<Callable>(f), std::make_index_sequence<sizeof...(Ts)>{});
#endif
   }
}
