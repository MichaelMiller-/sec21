#pragma once

#include <tuple>
#include <type_traits>
#include <variant>

namespace sec21::type_traits
{
   template <typename T, typename Ts>
   struct has_type;

   template <typename T, typename... Ts>
   struct has_type<T, std::tuple<Ts...>> : std::disjunction<std::is_same<T, Ts>...>
   {
   };

   template <typename T, typename... Ts>
   struct has_type<T, std::variant<Ts...>> : std::disjunction<std::is_same<T, Ts>...>
   {
   };

   namespace compile_time_tests
   {
      using test_tuple_t = std::tuple<int, float, double>;

      static_assert(has_type<int, test_tuple_t>::value == true);
      static_assert(has_type<float, test_tuple_t>::value == true);
      static_assert(has_type<double, test_tuple_t>::value == true);
      static_assert(has_type<char, test_tuple_t>::value == false);

      using test_variant_t = std::variant<int, float, double>;

      static_assert(has_type<int, test_variant_t>::value == true);
      static_assert(has_type<float, test_variant_t>::value == true);
      static_assert(has_type<double, test_variant_t>::value == true);
      static_assert(has_type<char, test_variant_t>::value == false);
   } // namespace compile_time_tests
} // namespace sec21::type_traits
