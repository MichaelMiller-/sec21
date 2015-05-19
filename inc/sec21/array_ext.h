#ifndef SEC21_ARRAY_EXT_H
#define SEC21_ARRAY_EXT_H

#include <array>

namespace sec21
{
  namespace std_array_ext
  {
    namespace hidden
    {
      template<
        class T,
        unsigned long N,
        int D>
      struct lenght2_impl
      {
          static T apply(std::array<T, N> const& value) BOOST_NOEXCEPT
          {
              T t = (value[D-1] * value[D-1]);
              return t + lenght2_impl<T, N, D-1>::apply(value);
          }
      };

      template<
        class T,
        unsigned long N>
      struct lenght2_impl<T, N, 0>
      {
          static T apply(std::array<T, N> const& value) BOOST_NOEXCEPT
          {
            return 0;
          }
      };

      template<
        class T,
        unsigned long N>
      inline T length2(std::array<T, N> const& value) BOOST_NOEXCEPT
      {
        return lenght2_impl<T, N, N>::apply(value);
      }

      template<
        class T,
        unsigned long N>
      inline T length(std::array<T, N> const& value) { return std::sqrt(lenght2(value)); }
      
    } // namespace hidden

    template<
      class T,
      unsigned long N>
    bool operator < (
      std::array<T, N> const& u,
      std::array<T, N> const& v)
    {
      return (hidden::length(u) < hidden::length(v));
    }

    template<
      class T,
      unsigned long N>
    inline const T* value_ptr(std::array<T, N> const& t) BOOST_NOEXCEPT
    {
      return t.data();
    }

    template<
      class T,
      unsigned long N>
    inline const T* value_ptr(std::vector<std::array<T, N> > const& t) BOOST_NOEXCEPT
    {
      return value_ptr(t[0]);
    }

    namespace math
    {
      template<
        class T>
      auto operator - (
        std::array<T, 2> const& u,
        std::array<T, 2> const& v) -> std::array<T, 2>
      {
        std::array<T, 2> result = { { u[0] - v[0], u[1] - v[1], } }; 
        return result;
      }  

      template<
        class T>
      auto operator - (
        std::array<T, 3> const& u,
        std::array<T, 3> const& v) -> std::array<T, 3>
      {
        std::array<T, 3> result = { { 
          u[0] - v[0], 
          u[1] - v[1], 
          u[2] - v[2], 
        } }; 
        return result;
      }  

      template<
        class T>
      auto operator - (
        std::array<T, 4> const& u,
        std::array<T, 4> const& v) -> std::array<T, 4>
      {
        std::array<T, 4> result = { { 
          u[0] - v[0], 
          u[1] - v[1], 
          u[2] - v[2], 
          static_cast<T>(0) 
        } }; 
        return result;
      }  

      template<
        class T>
      auto cross(
        std::array<T, 4> const& u,
        std::array<T, 4> const& v) -> std::array<T, 4>
      {
        std::array<T, 4> result = { { 
          u[1] * v[2] - u[1] * v[2],
          u[2] * v[0] - u[0] * v[2],
          u[0] * v[1] - u[1] * v[0],
          static_cast<T>(0) 
        } }; 
        return result;
      }
    } // namespace math
  }
}

#endif