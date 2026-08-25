#pragma once

#include <sec21/meta/push_front.h>

#include <type_traits>

namespace sec21::meta
{
   namespace detail
   {
      template <typename T, typename SortedList, template <typename, typename> class Less>
      struct insert_sorted;

      template <typename T, template <typename...> class List, template <typename, typename> class Less>
      struct insert_sorted<T, List<>, Less>
      {
         using type = List<T>;
      };

      template <typename T, template <typename...> class List, typename Head, typename... Tail,
                template <typename, typename> class Less>
      struct insert_sorted<T, List<Head, Tail...>, Less>
      {
      private:
         using inserted_tail = typename insert_sorted<T, List<Tail...>, Less>::type;

      public:
         // Equivalent values are inserted before the existing value, keeping sort stable.
         using type = std::conditional_t<Less<Head, T>::value, typename push_front<Head, inserted_tail>::type,
                                         List<T, Head, Tail...>>;
      };
   }

   template <typename List, template <typename, typename> class Less>
   struct sort;

   template <template <typename...> class List, template <typename, typename> class Less>
   struct sort<List<>, Less>
   {
      using type = List<>;
   };

   template <template <typename...> class List, typename Head, typename... Tail,
             template <typename, typename> class Less>
   struct sort<List<Head, Tail...>, Less>
   {
   private:
      using sorted_tail_t = typename sort<List<Tail...>, Less>::type;

   public:
      using type = typename detail::insert_sorted<Head, sorted_tail_t, Less>::type;
   };
} // namespace sec21::meta
