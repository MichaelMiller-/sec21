#ifndef SEC21_ARRAY_EXT_H
#define SEC21_ARRAY_EXT_H

#include <array>
#include <vector>
#include <cmath>

#include <boost/config.hpp>

namespace sec21
{
  namespace array_ext
  {
      template<
         class T,
         unsigned long N>
      BOOST_CONSTEXPR const T* value_ptr(
         std::array<T, N> const& t) BOOST_NOEXCEPT
      {
         return &t[0];
      }

      template<
         class T,
         unsigned long N>
      BOOST_CONSTEXPR const T* value_ptr(
         std::vector<std::array<T, N> > const& t) BOOST_NOEXCEPT
      {
         return value_ptr(t[0]);
      }

      namespace math
      {
         namespace hidden
         {
            template<
               class T,
               unsigned long N,
               int D>
            struct length2_impl
            {
               static T apply(std::array<T, N> const& value) BOOST_NOEXCEPT
               {
                  T t = (value[D-1] * value[D-1]);
                  return t + length2_impl<T, N, D-1>::apply(value);
               }
            };

            template<
               class T,
               unsigned long N>
            struct length2_impl<T, N, 0>
            {
               static T apply(std::array<T, N> const& value) BOOST_NOEXCEPT
               {
                  return 0;
               }
            };

         } // namespace hidden

         template<
            class T,
            unsigned long N>
         inline T length2(std::array<T, N> const& value) BOOST_NOEXCEPT
         {
            return hidden::length2_impl<T, N, N>::apply(value);
         }

         template<
            class T,
            unsigned long N>
         inline T length(std::array<T, N> const& value) 
         {
            return std::sqrt(length2(value));
         }

         template<
            class T,
            unsigned long N>
         bool geometric_less(
            std::array<T, N> const& u,
            std::array<T, N> const& v) BOOST_NOEXCEPT
         {
            return (length(u) < length(v));
         }

         template<
            class T>
         auto operator - (
            std::array<T, 2> const& u,
            std::array<T, 2> const& v) -> std::array<T, 2>
         {
            return std::array<T, 2>{ { u[0] - v[0], u[1] - v[1], } }; 
         }  

         template<
            class T>
         auto operator - (
            std::array<T, 3> const& u,
            std::array<T, 3> const& v) -> std::array<T, 3>
         {
            return std::array<T, 3>{ { 
               u[0] - v[0], 
               u[1] - v[1], 
               u[2] - v[2], } }; 
         }  

         template<
            class T>
         auto operator - (
            std::array<T, 4> const& u,
            std::array<T, 4> const& v) -> std::array<T, 4>
         {
            return std::array<T, 4>{ { 
               u[0] - v[0], 
               u[1] - v[1], 
               u[2] - v[2], 
               static_cast<T>(0) } }; 
         }  

         template<
            class T>
         auto cross(
            std::array<T, 4> const& u,
            std::array<T, 4> const& v) -> std::array<T, 4>
         {
            return std::array<T, 4>{ { 
               u[1] * v[2] - u[1] * v[2],
               u[2] * v[0] - u[0] * v[2],
               u[0] * v[1] - u[1] * v[0],
               static_cast<T>(0) } }; 
         }
      } // namespace math
   }
}

#endif