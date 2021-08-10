#pragma once

#include <boost/qvm/vec_traits_defaults.hpp>

#include <array>

namespace boost::qvm
{
   template <typename T, auto N>
   struct vec_traits<std::array<T, N>> : vec_traits_defaults<std::array<T, N>, T, N>
   {
      using scalar_type = T;

      template <auto I>
      static inline scalar_type& write_element(std::array<T, N>& v)
      {
         return std::get<I>(v);
      }
   };
} // namespace boost::qvm
