#pragma once

#include <type_traits>

namespace sec21::type_traits
{
   template <template <typename...> typename Base, typename U>
   struct is_instance_of : std::false_type
   {
   };

   template <template <typename...> typename Base, typename... Args>
   struct is_instance_of<Base, Base<Args...>> : std::true_type
   {
   };

   template <template <typename...> typename Base, typename U>
   inline constexpr bool is_instance_of_v = is_instance_of<Base, U>::value;

} // namespace sec21::type_traits
