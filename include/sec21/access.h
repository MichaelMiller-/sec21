#pragma once

#include <array>

namespace sec21
{
   namespace customization_point
   {
      template <typename T>
      struct access_x;

      template <typename T>
      struct access_y;

      template <typename T>
      struct access_z;      

      template <typename T, auto N>
      struct access_x<std::array<T, N>>
      {
         auto operator()(std::array<T, N> const& t) const noexcept
         {
            return std::get<0>(t);
         }
         auto& operator()(std::array<T, N>& t) const noexcept
         {
            return std::get<0>(t);
         }        
      };

      template <typename T, auto N>
      struct access_y<std::array<T, N>>
      {
         auto operator()(std::array<T, N> const& t) const noexcept
         {
            return std::get<1>(t);
         }
         auto& operator()(std::array<T, N>& t) const noexcept
         {
            return std::get<1>(t);
         }        
      };

      template <typename T, auto N>
      struct access_z<std::array<T, N>>
      {
         auto operator()(std::array<T, N> const& t) const noexcept
         {
            return std::get<2>(t);
         }
         auto& operator()(std::array<T, N>& t) const noexcept
         {
            return std::get<2>(t);
         }        
      };
   }

   template <typename T> 
   constexpr decltype(auto) X(T&& t) noexcept
   {
      return customization_point::access_x<std::decay_t<T>>()(std::forward<T>(t));
   }

   template <typename T> 
   constexpr decltype(auto) Y(T&& t) noexcept
   {
      return customization_point::access_y<std::decay_t<T>>()(std::forward<T>(t));
   }

   template <typename T> 
   constexpr decltype(auto) Z(T&& t) noexcept
   {
      return customization_point::access_z<std::decay_t<T>>()(std::forward<T>(t));
   }

} // namespace sec21