#pragma once

#include <tuple>
#include <functional>

//! \todo possible to replace with boost::function_types
namespace sec21 
{
   template <typename T>
   struct function_traits;

   template <typename R, typename... Args>
   struct function_traits<std::function<R(Args...)>>
   {
      static const size_t nargs = sizeof...(Args);

      typedef R result_type;

      template <size_t i>
      struct arg
      {
         typedef typename std::tuple_element<i, std::tuple<Args...>>::type type;
      };
   };
}
