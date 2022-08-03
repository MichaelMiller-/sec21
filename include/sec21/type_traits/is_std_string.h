#pragma once

#include <type_traits>

namespace sec21::type_traits
{
   namespace detail
   {
      [[maybe_unused]] std::true_type is_std_string(std::string);
      [[maybe_unused]] std::false_type is_std_string(...);
   } // namespace detail

   template <typename T>
   using is_std_string = decltype(detail::is_std_string(std::declval<T>()));

   template <typename T>
   static constexpr auto is_std_string_v = is_std_string<T>::value;

} // namespace sec21::type_traits