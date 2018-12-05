#pragma once

#include <type_traits>
 	
// #include <boost/qvm/quat.hpp>

namespace sec21::structural_analysis
{
   enum class support_kind_t : std::uint8_t
   {
      Free = 0,
      // reaction only in the y-axis
      Roller = 1,
      // reactions in the x & y axis
      Hinge,
      // reactions in the x & y axis and one moment as reaction
      Fixed
   };

   //! \todo a support is a kind and a rotation aka boost::qvm::quat<double>

   constexpr auto reactions(support_kind_t s) noexcept
   {
      return static_cast<std::underlying_type<decltype(s)>::type>(s);
   }
} // namespace sec21::structural_analysis
