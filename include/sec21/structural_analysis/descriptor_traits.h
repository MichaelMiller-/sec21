#pragma once

#include <sec21/type_traits/is_specialized.h>

#include <limits>
#include <string>

namespace sec21::structural_analysis
{
   template <typename T>
   struct descriptor_traits;

   template <typename T>
   struct is_descriptor : type_traits::is_specialized<descriptor_traits, T>
   {
   };

   template <typename T>
   static constexpr bool is_descriptor_v = is_descriptor<T>::value;

#ifdef __cpp_concepts
   template <typename T>
   concept Descriptor = is_descriptor<T>::value;
#endif

   // default traits <int, std::string>
   //
   template <>
   struct descriptor_traits<int>
   {
      using type_t = int;

      static constexpr auto invalid() noexcept -> type_t { return std::numeric_limits<type_t>::max(); }
   };

   template <>
   struct descriptor_traits<std::size_t>
   {
      using type_t = std::size_t;

      static constexpr auto invalid() noexcept -> type_t { return std::numeric_limits<type_t>::max(); }
   };

   template <>
   struct descriptor_traits<std::string>
   {
      using type_t = std::string;

      static type_t invalid() noexcept { return {""}; }
   };
} // namespace sec21::structural_analysis
