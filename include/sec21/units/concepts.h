#pragma once

#include <type_traits>

namespace sec21::units
{
   // forward declaration
   template <typename BaseDimension, auto Num, auto Denom>
   struct exponent;

   template <typename... Exponents>
   struct dimension;

   template <typename Unit, typename T>
   class quantity;

   // type traits
   template <typename T>
   struct is_exponent : std::false_type {};

   template <typename BaseDimension, auto Num, auto Denom>
   struct is_exponent<exponent<BaseDimension, Num, Denom>> : std::true_type {};

   template <typename T>
   struct is_quantity : std::false_type {};

   template <typename Unit, typename T>
   struct is_quantity<quantity<Unit, T>> : std::true_type {};

   template <typename T>
   struct is_dimension : std::false_type {};

   template <typename... Exponents>
   struct is_dimension<dimension<Exponents...>> : std::true_type {};

   template <typename T> // Quantity
   using to_dimension_t = dimension<typename T::dimension_t>;

   template <typename T, typename U>
   using has_same_dimension = std::is_same<to_dimension_t<T>, to_dimension_t<U>>;

#ifdef __cpp_concepts
   template <typename T>
   concept Scalar = std::is_scalar<T>::value;

   template <typename T>
   concept Exponent = is_exponent<T>::value;

   template <typename T>
   concept Dimension = is_dimension<T>::value;

   template <typename T>
   concept Quantity = is_quantity<T>::value;

   template <typename T, typename U>
   concept SameDimension = std::is_same<to_dimension_t<T>, to_dimension_t<U>>::value;
#endif   
}