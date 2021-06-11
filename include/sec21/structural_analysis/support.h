#pragma once

#include <array>
#include <type_traits>

namespace sec21::structural_analysis
{
   // basically tells in which axis (x,y,...,Dimension) the node is supported
   //! \todo kopplung
   //! \todo steifigkeit -> feder
   template <auto Dimension> //, typename Precision = double>
   using support = std::array<bool, Dimension>;

} // namespace sec21::structural_analysis
