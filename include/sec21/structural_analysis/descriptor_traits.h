#pragma once

#include <string>
#include <limits>

namespace sec21::structural_analysis
{
   template <typename T>
   struct descriptor_traits;

   // default traits <int, std::string>
   //
   template <>
   struct descriptor_traits<int>
   {
      using type_t = int;

      static constexpr auto invalid() noexcept -> type_t
      {
         return std::numeric_limits<type_t>::max();
      }
   };

   template <>
   struct descriptor_traits<std::size_t>
   {
      using type_t = std::size_t;

      static constexpr auto invalid() noexcept -> type_t
      {
         return std::numeric_limits<type_t>::max();
      }
   };

   template <>
   struct descriptor_traits<std::string>
   {
      using type_t = std::string;

      static type_t invalid() noexcept
      {
         return {""};
      }
   };
}
