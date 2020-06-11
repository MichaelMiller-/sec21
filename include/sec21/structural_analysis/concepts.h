#pragma once

#include <type_traits>
// #include <sec21/units.h>

namespace sec21::structural_analysis
{
   // forward declaration
   //! \todo units::Scalar
   template <auto Dimension, typename Precision>
   struct node;

   struct space_truss;

   // type traits
   template <typename T>
   struct is_node : std::false_type {};

   template <auto Dimension, typename Precision>
   struct is_node<node<Dimension, Precision>> : std::true_type {};

   template <typename T>
   constexpr bool is_node_v = is_node<T>::value;

   template <typename T>
   struct is_space_truss : std::is_same<T, space_truss> {};

   template <typename T>
   struct is_space_truss_2D : std::conjunction<is_space_truss<T>, std::bool_constant<T::dimension_v == 2>> {};

   // concepts
   template <typename T>
   concept Node = is_node<T>::value;

   template <typename T>
   concept SpaceTruss = is_space_truss<T>::value;

   template <typename T>
   concept SpaceTruss2D = SpaceTruss<T> && is_space_truss_2D<T>::value;
}