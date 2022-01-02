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
   constexpr auto pi = std::numbers::pi_v<T>;
#else
   template <typename T>
   constexpr auto pi = boost::math::constants::pi<T>();
#endif

} // namespace sec21::structural_analysis