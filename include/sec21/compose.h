#pragma once

#include <sec21/type_traits/function_traits.h>
#include <sec21/type_traits/is_optional.h>

namespace sec21
{
   // g: A -> B, f: B --> C
   auto compose(auto f, auto g)
   {
      return [f, g](auto&&... args) { return std::invoke(f, std::invoke(g, std::forward<decltype(args)>(args)...)); };
   }

   template <typename T>
   static constexpr auto is_return_type_optional_v =
      type_traits::is_optional_v<typename type_traits::function_traits<std::remove_pointer_t<T>>::result_type>;

   // g: A -> optional<B>, f: B -> optional<C>
   template <typename Callable>
      requires is_return_type_optional_v<Callable>
   auto compose(Callable f, Callable g)
   {
      return [f, g](auto&&... args) -> type_traits::function_traits<std::remove_pointer_t<decltype(f)>>::result_type {
         auto result_g = std::invoke(g, std::forward<decltype(args)>(args)...);
         if (not result_g) {
            return std::nullopt;
         }
         return std::invoke(f, *result_g);
      };
   }
} // namespace sec21