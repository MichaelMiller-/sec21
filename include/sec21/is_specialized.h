#pragma once

#include <type_traits>

namespace sec21
{
   template <template <typename...> class, typename, typename = void>
   struct is_specialized : std::false_type
   {
   };

   template <template <typename...> class Template, typename T>
   //! \todo constrain to that "Template" is not constructable
   struct is_specialized<Template, T, std::void_t<decltype(Template<T>{})>> : std::true_type
   {
   };

   template <template <typename...> class Template, typename T>
   constexpr auto is_specialized_v = is_specialized<Template, T>::value;

} // namespace sec21
