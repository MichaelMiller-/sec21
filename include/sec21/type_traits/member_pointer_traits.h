#pragma once

#include <type_traits>

namespace sec21::type_traits
{
   namespace detail
   {
      template <typename T>
      struct decompose_member_pointer;

      template <typename T, typename U>
      struct decompose_member_pointer<T U::*>
      {
         using value_t = T;
         using class_t = U;
      };
   } // namespace detail

   template <typename T>
   struct member_pointer_traits : detail::decompose_member_pointer<std::remove_cv_t<T>>
   {
   };
} // namespace sec21::type_traits