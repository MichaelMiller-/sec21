#pragma once

#include <optional>

namespace sec21::type_traits
{
   template <typename T, typename = void>
   struct is_optional : std::false_type
   {
   };

   template <typename T>
   struct is_optional<std::optional<T>, void> : std::true_type
   {
   };

   template <typename T>
   inline constexpr auto is_optional_v = is_optional<T>::value;

} // namespace sec21::type_traits