#pragma once

#ifdef __cpp_lib_math_constants
#include <numbers>
#else
#include <boost/math/constants/constants.hpp>
#endif

namespace sec21::structural_analysis
{
#ifdef __cpp_lib_math_constants
   template <typename T>
   static constexpr auto pi = std::numbers::pi_v<T>;

   template <typename T>
   static constexpr auto half_pi = pi<T> * 0.5;
#else
   template <typename T>
   static constexpr auto pi = boost::math::constants::pi<T>();

   template <typename T>
   static constexpr auto half_pi = boost::math::constants::half_pi<T>();
#endif

   template <typename T>
   static constexpr auto quarter_pi = half_pi<T> * 0.5;

} // namespace sec21::structural_analysis