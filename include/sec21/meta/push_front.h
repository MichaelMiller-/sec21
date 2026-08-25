#pragma once

namespace sec21::meta
{
   template <typename T, typename List>
   struct push_front;

   template <typename T, template <typename...> class List, typename... Ts>
   struct push_front<T, List<Ts...>>
   {
      using type = List<T, Ts...>;
   };

   template <typename T, typename List>
   using push_front_t = push_front<T, List>::type;
} // namespace sec21::meta
