#pragma once

#include <type_traits>

namespace sec21::structural_analysis
{
   // forward declaration
   template <auto Dimension, typename Descriptor, typename Precision>
   struct node;

   template <typename Descriptor, typename Precision>
   struct member;

   template <typename Descriptor, typename Precision>
   struct space_truss;

   // type traits
   template <typename T>
   struct is_node : std::false_type {};

   template <auto Dimension, typename Descriptor, typename Precision>
   struct is_node<node<Dimension, Descriptor, Precision>> : std::true_type {};

   template <typename T>
   struct is_member : std::false_type {};

   template <typename Descriptor, typename Precision>
   struct is_member<member<Descriptor, Precision>> : std::true_type {};

   template <typename T>
   struct is_space_truss : std::false_type {};

   template <typename Descriptor, typename Precision>
   struct is_space_truss<space_truss<Descriptor, Precision>> : std::true_type {};

   template <typename T>
   struct is_space_truss_2D : std::conjunction<is_space_truss<T>, std::bool_constant<T::dimension_v == 2>> {};

#ifdef __cpp_concepts
   // concepts
   template <typename T>
   concept Node = is_node<T>::value;

   template <typename T>
   concept Member = is_member<T>::value;

   template <typename T>
   concept SpaceTruss = is_space_truss<T>::value;

   template <typename T>
   concept SpaceTruss2D = SpaceTruss<T> && is_space_truss_2D<T>::value;
#endif   
}