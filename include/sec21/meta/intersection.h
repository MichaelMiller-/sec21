#pragma once

#include <sec21/meta/contains.h>
#include <sec21/meta/push_front.h>

#include <type_traits>

namespace sec21::meta
{
   template <typename, typename, template <typename, typename...> class Compare = contains>
   struct intersection;

   template <template <typename...> class List, typename... Ts, template <typename, typename...> class Compare>
   struct intersection<List<>, List<Ts...>, Compare>
   {
      using type = List<>;
   };

   template <template <typename...> class List, typename First, typename... Rest, typename... Right,
             template <typename, typename...> class Compare>
   struct intersection<List<First, Rest...>, List<Right...>, Compare>
   {
   private:
      using rest_t = intersection<List<Rest...>, List<Right...>, Compare>::type;

   public:
      using type =
         std::conditional_t<Compare<First, Right...>::value, typename push_front<First, rest_t>::type, rest_t>;
   };
} // namespace sec21::meta
