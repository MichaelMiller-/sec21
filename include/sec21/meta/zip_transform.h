#pragma once

#include <sec21/meta/push_front.h>

namespace sec21::meta
{
   template <typename Left, typename Right, template <typename, typename> class Transform>
   struct zip_transform;

   template <template <typename...> class List, template <typename, typename> class Transform>
   struct zip_transform<List<>, List<>, Transform>
   {
      using type = List<>;
   };

   template <template <typename...> class List, typename LeftFirst, typename... LeftRest, typename RightFirst,
             typename... RightRest, template <typename, typename> class Transform>
   struct zip_transform<List<LeftFirst, LeftRest...>, List<RightFirst, RightRest...>, Transform>
   {
   private:
      using rest_t = typename zip_transform<List<LeftRest...>, List<RightRest...>, Transform>::type;

   public:
      using type = typename push_front<Transform<LeftFirst, RightFirst>, rest_t>::type;
   };
} // namespace sec21::meta
