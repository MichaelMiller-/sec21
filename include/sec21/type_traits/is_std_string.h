#pragma once

namespace sec21::type_traits
{
   namespace detail
   {
      [[maybe_unused]] std::true_type is_std_string(std::string);
      [[maybe_unused]] std::false_type is_std_string(...);
   }

   template<typename T>
   using is_std_string = decltype(detail::is_std_string(std::declval<T>()));

   template<typename T>
   inline constexpr auto is_std_string_v = is_std_string<T>::value;

   static_assert(is_std_string<std::string>::value);
   static_assert(is_std_string<int>::value == false);
   static_assert(is_std_string<std::wstring>::value == false);
   static_assert(is_std_string<float>::value == false);

   static_assert(is_std_string_v<std::string>);
   static_assert(is_std_string_v<float> == false);
   static_assert(is_std_string_v<int> == false);
}