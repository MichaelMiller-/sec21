#pragma once

namespace sec21::meta
{
   template <typename... Lists>
   struct merge;

   template <typename List>
   struct merge<List>
   {
      using type = List;
   };

   template <template <typename...> class List, typename... Left, typename... Right, typename... Remaining>
   struct merge<List<Left...>, List<Right...>, Remaining...>
   {
      using type = typename merge<List<Left..., Right...>, Remaining...>::type;
   };
} // namespace sec21::meta
