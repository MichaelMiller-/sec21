#pragma once

#include <utility>

namespace sec21::units
{
   template <typename BaseType>
   struct downcast_base 
   {
      using base_t = BaseType;
      friend auto downcast_guide(downcast_base);
   };

   template <typename Target, typename T>
   struct downcast_helper : T 
   {
      friend auto downcast_guide(typename downcast_helper::downcast_base) { return Target(); }
   };

   namespace detail
   {
      template <typename T>
      constexpr auto downcast_target_impl() 
      {
         return decltype(downcast_guide(std::declval<downcast_base<T>>()))();
      }
   }

   template <typename T>
   using downcast_target = decltype(detail::downcast_target_impl<T>());
}
