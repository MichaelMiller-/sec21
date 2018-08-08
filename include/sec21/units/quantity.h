#pragma once

#include <type_traits>

#include <sec21/units/io_traits.h>

namespace sec21::units
{
   template <typename Unit, typename T = double>
   struct quantity_t
   {
      using value_t = T;
      using unit_t = Unit;

      T m_value{};

      quantity_t() = default;

      template <typename U, class = typename std::enable_if<std::is_arithmetic<U>::value>::type>
      constexpr quantity_t(U const& value) noexcept
         : m_value{ value }
      { }

      template <class U, typename V>
      constexpr bool operator == (quantity_t<U, V> const& rhs) const noexcept
      {
         if constexpr (std::is_same_v<Unit::base_unit_t, U::base_unit_t>)
            return m_value == rhs.m_value;

         return m_value == (rhs.m_value * Unit::conversion_t::den * U::conversion_t::num / U::conversion_t::den);
      }
   };
}

#include <ostream>
//! \todo replace with std::basic_ostream<T>
template <typename Unit, typename T>
std::ostream& operator << (std::ostream& os, sec21::units::quantity_t<Unit, T> const& q) {
   return os << q.m_value << io_traits<Unit>::symbol();
}