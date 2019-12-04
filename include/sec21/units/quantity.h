#pragma once

#include <type_traits>
#include <limits>
#include <ratio>

#include <sec21/units/unit.h>
#include <sec21/units/ratio.h>

namespace sec21::units
{
   // forward declaration
   template <typename Unit, typename T>
   class quantity;

   template <typename To, typename Unit, typename T>
   [[nodiscard]] constexpr auto quantity_cast(quantity<Unit, T> const& q) noexcept
   {
      using cr = std::ratio_divide<typename Unit::ratio_t, typename To::unit_t::ratio_t>;
      return To{ static_cast<typename To::value_t>(q.value() * cr::num / cr::den) };
   }

   // the amount of a specific dimension expressed in a specific unit of that dimension
   template <typename Unit, typename T = double>
   class quantity
   {
      T m_value;

   public:
      using unit_t = Unit;
      using value_t = T;
      using dimension_t = typename Unit::dimension_t;

      quantity() = default;

      constexpr quantity(T const& t) noexcept
         : m_value{ static_cast<T>(t) }
      {}

      template <typename Q2, typename = std::enable_if<std::is_same_v<dimension_t, typename Q2::dimension_t>>>
      constexpr quantity(Q2 const& q) noexcept
         : m_value{ quantity_cast<quantity>(q).value() }
      {}

      constexpr auto value() const noexcept {
         return m_value;
      }

      //[[nodiscard]] static constexpr quantity zero() noexcept {
      //   //! \todo 
      //   return {};
      //}
      //// quantity equal to 1 expressed in a specific Rep
      ////! \todo 
      //[[nodiscard]] static constexpr quantity one() noexcept;

      [[nodiscard]] static constexpr quantity min() noexcept
      {
         return std::numeric_limits<value_t>::lowest();
      }
      [[nodiscard]] static constexpr quantity max() noexcept
      {
         return std::numeric_limits<value_t>::max();
      }
   };

   namespace detail
   {
      template <typename Q1, typename Q2, typename T>
      struct common_quantity_impl;

      template <typename U, typename T1, typename T2, typename T>
      struct common_quantity_impl<quantity<U, T1>, quantity<U, T2>, T>
      {
         using type = quantity<U, T>;
      };

      template <typename U1, typename T1, typename U2, typename T2, typename T>
      //requires same_dim<typename U1::dimension, typename U2::dimension>
      struct common_quantity_impl<quantity<U1, T1>, quantity<U2, T2>, T>
      {
         using cr = common_ratio_t<typename U1::ratio_t, typename U2::ratio_t>;
         using cu = unit<typename U1::dimension_t, cr>;
         using type = quantity<cu, T>;
         // using type = quantity<downcast_target<cu>, T>;
      };
   }

   template <typename Q1, typename Q2, typename T = std::common_type_t<typename Q1::value_t, typename Q2::value_t>>
   using common_quantity_t = typename detail::common_quantity_impl<Q1, Q2, T>::type;


   template <typename U1, typename T1, typename U2, typename T2>
   [[nodiscard]] constexpr bool operator == (quantity<U1, T1> const& lhs, quantity<U2, T2> const& rhs) noexcept
      //requires same_dim<typename U1::dimension, typename U2::dimension>
   {
      using ct = common_quantity_t<quantity<U1, T1>, quantity<U1, T2>>;
      return ct{ lhs }.value() == ct{ rhs }.value();
   }

   template <typename U1, typename T1, typename U2, typename T2>
   [[nodiscard]] constexpr bool operator != (quantity<U1, T1> const& lhs, quantity<U2, T2> const& rhs) noexcept
   {
      return !(lhs == rhs);
   }

   template <typename U1, typename T1, typename U2, typename T2>
   [[nodiscard]] constexpr bool operator < (quantity<U1, T1> const& lhs, quantity<U2, T2> const& rhs) noexcept
      //requires same_dim<typename U1::dimension, typename U2::dimension>
   {
      using ct = common_quantity_t<quantity<U1, T1>, quantity<U1, T2>>;
      return ct{ lhs }.value() < ct{ rhs }.value();
   }

   template <typename U1, typename T1, typename U2, typename T2>
   [[nodiscard]] constexpr bool operator <= (quantity<U1, T1> const& lhs, quantity<U2, T2> const& rhs) noexcept
      //requires same_dim<typename U1::dimension, typename U2::dimension>
   {
      return !(rhs < lhs);
   }

   template <typename U1, typename T1, typename U2, typename T2>
   [[nodiscard]] constexpr bool operator > (quantity<U1, T1> const& lhs, quantity<U2, T2> const& rhs) noexcept
      //requires same_dim<typename U1::dimension, typename U2::dimension>
   {
      return rhs < lhs;
   }

   template <typename U1, typename T1, typename U2, typename T2>
   [[nodiscard]] constexpr bool operator >= (quantity<U1, T1> const& lhs, quantity<U2, T2> const& rhs) noexcept
      //requires same_dim<typename U1::dimension, typename U2::dimension>
   {
      return !(lhs < rhs);
   }

   template <typename U1, typename T1, typename U2, typename T2>
   [[nodiscard]] constexpr auto operator + (quantity<U1, T1> const& lhs, quantity<U2, T2> const& rhs) noexcept
      //      requires same_dim<typename U1::dimension, typename U2::dimension>
   {
      using ct = common_quantity_t<quantity<U1, T1>, quantity<U2, T2>>;
      return ct{ ct{lhs}.value() + ct{rhs}.value() };
   }

   template <typename U1, typename T1, typename U2, typename T2>
   [[nodiscard]] constexpr auto operator - (quantity<U1, T1> const& lhs, quantity<U2, T2> const& rhs) noexcept
      //      requires same_dim<typename U1::dimension, typename U2::dimension>
   {
      using ct = common_quantity_t<quantity<U1, T1>, quantity<U2, T2>>;
      return ct{ lhs }.value() - ct{ rhs }.value();
   }

   //! \todo operator *

   template <typename U1, typename T1, typename U2, typename T2>
   // require U1::dimension_t != U2::dimension_t
   [[nodiscard]] constexpr auto operator * (quantity<U1, T1> const& lhs, quantity<U2, T2> const& rhs)
   {
      using vt = decltype(lhs.value() / rhs.value());
      using dt = multiply_dimension_t<typename U1::dimension_t, typename U2::dimension_t>;
      using rt = quantity<unit<dt, std::ratio_multiply<typename U1::ratio_t, typename U2::ratio_t>>, vt>;
      return rt{ lhs.value() * rhs.value() };
   }

   //! \todo 
   // require same_dimension -> return Scalar
   // [[nodiscard]] constexpr auto operator / (quantity<U1, T1> const& lhs, quantity<U2, T2> const& rhs)

   template <typename U1, typename T1, typename U2, typename T2>
   // require U1::dimension_t != U2::dimension_t
   [[nodiscard]] constexpr auto operator / (quantity<U1, T1> const& lhs, quantity<U2, T2> const& rhs)
   {
      using vt = decltype(lhs.value() / rhs.value());
      using dt = divide_dimension_t<typename U1::dimension_t, typename U2::dimension_t>;
      using rt = quantity<unit<dt, std::ratio_divide<typename U1::ratio_t, typename U2::ratio_t>>, vt>;
      return rt{ lhs.value() / rhs.value() };
   }

   template <typename U1, typename T1, typename T2>
   [[nodiscard]] constexpr auto operator / (quantity<U1, T1> const& lhs, T2 const& rhs)
   {
      // [[expects: rhs != quantity<U2, T2>(0)]]
      using vt = decltype(lhs.value() / rhs);
      using rt = quantity<U1, vt>;
      return rt{ rt{ lhs }.value() / rhs };
   }

   template <typename T1, typename U2, typename T2>
   [[nodiscard]] constexpr auto operator / (T1 const& lhs, quantity<U2, T2> const& rhs)
   {
      // [[expects: rhs != quantity<U2, T2>(0)]]
      using vt = decltype(lhs / rhs.value());
      using dt = invert_dimension_t<typename U2::dimension_t>;
      using rt = quantity<unit<dt, std::ratio<U2::ratio_t::den, U2::ratio_t::num>>, vt>;
      return rt{ lhs / rhs.value() };
   }

}
