#pragma once

namespace sec21
{
   //! \todo customization point -> boost::geometry, ..
   //! \todo write access -> auto& 

   template <typename T> 
   constexpr auto X(T&& t) noexcept
   {
      return std::get<0>(std::forward<T>(t));
   }

   template <typename T> 
   constexpr auto Y(T&& t) noexcept
   {
      return std::get<1>(std::forward<T>(t));
   }

   template <typename T> 
   constexpr auto Z(T&& t) noexcept
   {
      return std::get<2>(std::forward<T>(t));
   }

} // namespace sec21