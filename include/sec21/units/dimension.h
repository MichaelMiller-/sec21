#pragma once

#include <boost/mp11.hpp>

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

   template <typename D1, typename D2>
      struct base_dimension_less : std::bool_constant < D1::name<D2::name>
   {
   };

   template <typename D1, typename D2>
   static constexpr bool base_dimension_less_v = base_dimension_less<D1, D2>::value;

   template <typename Base>
   struct upcast_base
   {
      using base_t = Base;
   };

   // a typelist-type that stores an ordered list of exponents of one or more base_dimension
   template <typename... Exponents>
   struct dimension : upcast_base<dimension<Exponents...>>
   {
   };

   // ! \todo possible solution: P0732 class types in non-type template parameter

   // a base dimension and it's exponent in a derived dimension
   template <typename BaseDimension, auto Num, auto Denom = 1>
   struct exponent
   {
      //! \todo remove dimension_t
      using dimension_t = BaseDimension;
      using tag_t = BaseDimension;

      static constexpr auto num = Num;
      static constexpr auto den = Denom;
   };

   template <typename E>
   struct is_numerator_null : std::bool_constant<E::num == 0>
   {
   };

   template <typename E1, typename E2>
   using same_dimension = std::is_same<dimension<typename E1::dimension_t>, dimension<typename E2::dimension_t>>;

   template <typename E1, typename E2>
   inline constexpr bool same_dimension_v = same_dimension<E1, E2>::value;

   template <typename E1, typename E2>
   struct dimension_less : base_dimension_less<typename E1::dimension_t, typename E2::dimension_t>
   {
   };

   namespace detail
   {
      template <typename E1, typename E2>
      struct add_exponent_impl
      {
         static_assert(std::is_same_v<typename E1::dimension_t, typename E2::dimension_t>, "why");
         using type = exponent<typename E1::dimension_t, E1::num + E2::num>;
      };

   } // namespace detail

   template <typename E1, typename E2>
   // requires std::is_same_v<typename E1::dimension_t, typename E2::dimension_t>
   using add_exponent_t = typename detail::add_exponent_impl<E1, E2>::type;

   template <typename T>
   using invert_exponent_t = exponent<typename T::dimension_t, -T::num, T::den>;

   namespace detail
   {
      template <typename U>
      struct quoted_same_dimension
      {
         template <typename T>
         using fn = same_dimension<T, U>;
      };
      //! \todo could ne inside multiply_dimension_impl<>
      template <typename L>
      struct dimension_in_list
      {
         template <typename T>
         using fn = boost::mp11::mp_count_if_q<L, quoted_same_dimension<T>>;
      };
      //! \todo could ne inside multiply_dimension_impl<>
      struct quoted_exponent_add
      {
         template <typename E1, typename E2>
         using fn = add_exponent_t<E1, E2>;
      };

      template <typename D1, typename D2>
      struct multiply_dimension_impl
      {
         using L1 = boost::mp11::mp_sort<boost::mp11::mp_copy_if_q<D1, dimension_in_list<D2>>, dimension_less>;
         using L2 = boost::mp11::mp_sort<boost::mp11::mp_copy_if_q<D2, dimension_in_list<D1>>, dimension_less>;

         using T1 = boost::mp11::mp_transform_q<quoted_exponent_add, L1, L2>;

         using R1 = boost::mp11::mp_remove_if<T1, is_numerator_null>;
         using R2 = boost::mp11::mp_remove_if_q<D1, dimension_in_list<D2>>;
         using R3 = boost::mp11::mp_remove_if_q<D2, dimension_in_list<D1>>;

         using type = boost::mp11::mp_sort<boost::mp11::mp_append<R1, R2, R3>, dimension_less>;
      };
   } // namespace detail

   template <typename D1, typename D2>
   using multiply_dimension_t =
      typename detail::multiply_dimension_impl<typename D1::base_t, typename D2::base_t>::type;

   namespace detail
   {
      template <typename D1, typename D2>
      struct divide_dimension_impl;

      template <typename... E1, typename... E2>
      struct divide_dimension_impl<dimension<E1...>, dimension<E2...>>
          : multiply_dimension_impl<dimension<E1...>, dimension<invert_exponent_t<E2>...>>
      {
      };
   } // namespace detail

   template <typename D1, typename D2>
   using divide_dimension_t = typename detail::divide_dimension_impl<typename D1::base_t, typename D2::base_t>::type;

   namespace detail
   {
      template <typename D>
      struct invert_dimension_impl;

      template <typename... D>
      struct invert_dimension_impl<dimension<D...>> : boost::mp11::mp_identity<dimension<invert_exponent_t<D>...>>
      {
      };
   } // namespace detail

   template <typename D>
   using invert_dimension_t = typename detail::invert_dimension_impl<typename D::base_t>::type;
} // namespace sec21::units
