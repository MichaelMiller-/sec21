#pragma once

#include <boost/qvm/vec_traits_defaults.hpp>

#include <array>

namespace boost::qvm
{
   //! \brief vec_traits specialization that allows Boost QVM functions to operate directly on arrays of type std::array
   template <typename T, auto N>
   struct vec_traits<std::array<T, N>> : vec_traits_defaults<std::array<T, N>, T, N>
   {
      template <auto I>
      [[nodiscard]] static inline auto write_element(std::array<T, N>& obj) -> auto&
      {
         return std::get<I>(obj);
      }
   };
} // namespace boost::qvm
