#pragma once

#include <type_traits>

namespace sec21::type_traits
{
   template <template <typename...> class, typename, typename = void>
   struct is_specialized : std::false_type
   {
   };

   template <template <typename...> class Template, typename T>
   struct is_specialized<Template, T, std::void_t<decltype(Template<T>{})>> : std::true_type
   {
   };

   template <template <typename...> class Template, typename T>
   inline constexpr bool is_specialized_v = is_specialized<Template, T>::value;

} // namespace sec21
