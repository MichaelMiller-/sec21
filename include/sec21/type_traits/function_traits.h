#pragma once

#include <tuple>

namespace sec21::type_traits
{
   template <typename>
   struct function_traits;

   template <typename ClassType, typename Result, typename... Args>
   struct function_traits<Result (ClassType::*)(Args...) const>
   {
      using result_type = Result;

      template <std::size_t Index>
      using argument_type = typename std::tuple_element<Index, std::tuple<Args...>>::type;

      static constexpr auto arity = sizeof...(Args);
   };

   template <typename Result, typename... Args>
   struct function_traits<Result(Args...)>
   {
      using result_type = Result;

      template <std::size_t Index>
      using argument_type = typename std::tuple_element<Index, std::tuple<Args...>>::type;

      static constexpr auto arity = sizeof...(Args);
   };
} // namespace sec21::type_traits