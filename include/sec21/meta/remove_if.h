#pragma once

#include <sec21/meta/push_front.h>

#include <type_traits>

namespace sec21::meta
{
   template <typename List, template <typename> class Predicate>
   struct remove_if;

   template <template <typename...> class List, template <typename> class Predicate>
   struct remove_if<List<>, Predicate>
   {
      using type = List<>;
   };

   template <template <typename...> class List, typename First, typename... Rest, template <typename> class Predicate>
   struct remove_if<List<First, Rest...>, Predicate>
   {
   private:
      using rest_t = typename remove_if<List<Rest...>, Predicate>::type;

   public:
      using type = std::conditional_t<Predicate<First>::value, rest_t, typename push_front<First, rest_t>::type>;
   };
} // namespace sec21::meta
