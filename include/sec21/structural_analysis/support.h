#pragma once

#include <type_traits>
#include <array>

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

   // basically tells in which axis (x,y,...,Dimension) the node is supported
   //! \todo kopplung
   //! \todo steifigkeit -> feder
   template <auto Dimension> //, typename Precision = double>
   using support = std::array<bool, Dimension>;

   //! \return Anzahl der Freiheitsgrade
   [[deprecated]] constexpr auto variability(const Support s) noexcept
   {
      return static_cast<std::underlying_type<decltype(s)>::type>(s);
   }
} // namespace sec21::structural_analysis

