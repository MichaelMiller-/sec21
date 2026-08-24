#pragma once

#include <sec21/meta/difference.h>
#include <sec21/meta/intersection.h>
#include <sec21/meta/merge.h>
#include <sec21/meta/remove_if.h>
#include <sec21/meta/sort.h>
#include <sec21/meta/zip_transform.h>

namespace sec21::units
{
   // base_dimension is a unique sortable compile-time value
   //! \todo is more like a dimension_tag
   template <auto Name, auto Symbol = 1>
   struct base_dimension
   {
      static constexpr auto name = Name;
      static constexpr auto symbol = Symbol;
   };

   // clang-format off
   template <typename D1, typename D2>
   struct base_dimension_less : std::bool_constant<D1::name < D2::name>
   {
   };
   // clang-format on

   // a type-list that stores an ordered list of exponents of one or more base_dimension
   template <typename...>
   struct dimension
   {
   };

   // ! \todo possible solution: P0732 class types in non-type template parameter

   // a base dimension and it's exponent in a derived dimension
   template <typename BaseDimension, auto Numerator, auto Denominator = 1>
   struct exponent
   {
      using base_t = BaseDimension;

      static constexpr auto numerator_v = Numerator;
      static constexpr auto denominator_v = Denominator;
   };

   template <typename Lhs, typename Rhs>
   struct exponent_less : base_dimension_less<typename Lhs::base_t, typename Rhs::base_t>
   {
   };

   template <typename Lhs, typename Rhs>
   struct exponent_same_base : std::is_same<typename Lhs::base_t, typename Rhs::base_t>
   {
   };

   template <typename T, typename... Ts>
   struct exponent_contains_same_base : std::disjunction<exponent_same_base<T, Ts>...>
   {
   };

   template <typename E>
   struct is_numerator_null : std::bool_constant<E::numerator_v == 0>
   {
   };

   template <typename E1, typename E2>
   struct dimension_less : base_dimension_less<typename E1::base_t, typename E2::base_t>
   {
   };

   template <typename E1, typename E2>
      requires std::is_same_v<typename E1::base_t, typename E2::base_t>
   using add_exponent_t = exponent<typename E1::base_t, E1::numerator_v + E2::numerator_v>;

   template <typename T>
   using invert_exponent_t = exponent<typename T::base_t, -T::numerator_v, T::denominator_v>;

   template <typename Lhs, typename Rhs>
   struct multiply_dimension
   {
      // clang-format off
         using type = meta::sort<
            typename meta::merge<
               typename meta::remove_if<
                  typename meta::zip_transform<
                     typename meta::sort<
                        typename meta::intersection<Lhs, Rhs, exponent_contains_same_base>::type, exponent_less>::type,
                     typename meta::sort<
                        typename meta::intersection<Rhs, Lhs, exponent_contains_same_base>::type, exponent_less>::type,
                     add_exponent_t>::type,
                  is_numerator_null>::type,
               typename meta::difference<Lhs, Rhs, exponent_contains_same_base>::type,
               typename meta::difference<Rhs, Lhs, exponent_contains_same_base>::type>::type,
            dimension_less>::type;
      // clang-format on
   };

   template <typename Lhs, typename Rhs>
   using multiply_dimension_t = multiply_dimension<Lhs, Rhs>::type;

   template <typename D1, typename D2>
   struct divide_dimension;

   template <typename... E1, typename... E2>
   struct divide_dimension<dimension<E1...>, dimension<E2...>>
       : multiply_dimension<dimension<E1...>, dimension<invert_exponent_t<E2>...>>
   {
   };

   template <typename Lhs, typename Rhs>
   using divide_dimension_t = divide_dimension<Lhs, Rhs>::type;

   template <typename D>
   struct invert_dimension;

   template <typename... D>
   struct invert_dimension<dimension<D...>> : std::type_identity<dimension<invert_exponent_t<D>...>>
   {
   };

   template <typename D>
   using invert_dimension_t = invert_dimension<D>::type;

} // namespace sec21::units
