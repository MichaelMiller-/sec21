#pragma once

#include <sec21/fixed_string.h>

#include <tuple>
#include <string_view>

namespace sec21::database
{
   struct primary_key {
      static constexpr inline auto value = "PRIMARY KEY";
   };
   struct not_null {
      static constexpr inline auto value = "NOT NULL";
   };
   struct unique {
      static constexpr inline auto value = "UNIQUE";
   };


   struct column_base {};

   template <fixed_string Name, typename Class, typename T, T Class::*ptr, typename... Constraints>
   struct column : column_base
   {
      using class_t = Class;    
      using value_t = T;
      using member_ptr_t = value_t class_t::*;    
      using constraints_t = std::tuple<Constraints...>;

      static constexpr std::string_view name{ Name };

      static auto& get(Class& obj) noexcept
      {
         return obj.*ptr;
      }
      static auto get(Class const& obj) noexcept
      {
         return obj.*ptr;
      }
   };

   template <typename T>
   struct is_column : std::is_base_of<column_base, T> {};

   template <typename T>
   inline constexpr bool is_column_v = is_column<T>::value;

#ifdef __cpp_concepts
   template<typename T>
   concept Column = is_column_v<T>;
#endif   
}