#pragma once

namespace sec21::structural_analysis
{
   struct space_truss;

   template <typename T>
   struct is_space_truss : std::is_same<T, space_truss> {};

   template <typename T>
   struct is_space_truss_2D : std::conjunction<is_space_truss<T>, std::bool_constant<T::dimension_v == 2>> {};

#ifdef __cpp_concepts
   template <typename T>
   concept SpaceTruss = is_space_truss<T>::value;

   template <typename T>
   concept SpaceTruss2D = SpaceTruss<T> && is_space_truss_2D<T>::value;
#endif
}