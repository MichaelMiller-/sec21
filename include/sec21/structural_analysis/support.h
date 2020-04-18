#pragma once

#include <type_traits>

namespace sec21::structural_analysis
{
   enum class Support
   {
      // reaction only in the y-axis
      Roller = 1,
      // reactions in the x & y axis
      Hinge = 2,
      // reactions in the x & y axis and one moment as reaction
      Fixed = 3
   };


   //! \return Anzahl der Freiheitsgrade
   constexpr auto variability(Support s) noexcept
   {
      return static_cast<std::underlying_type<decltype(s)>::type>(s);
   }
} // namespace sec21::structural_analysis

