#pragma once

#include <sec21/units/dimension.h>
#include <sec21/units/io_traits.h>
#include <sec21/units/quantity.h>

namespace sec21::units
{
   namespace length
   {
      using meter = dimension_t<std::ratio<1>, category::length>;       // meters are (1) unit of length in the SI system.
      using centimeter = dimension_t<std::ratio<1, 100>, meter>;
      using kilometer = dimension_t<std::ratio<1000, 1>, meter>;
      using feet = dimension_t<std::ratio<381, 1250>, meter>;           // feet are 0.3048 meters.
      using yard = dimension_t<std::ratio<3>, feet>;
   }

   
   template <> struct io_traits<length::meter>
   {
      static constexpr auto symbol() { return "m"; }
   };
   template <> struct io_traits<length::centimeter>
   {
      static constexpr auto symbol() { return "cm"; }
   };
   template <> struct io_traits<length::kilometer>
   {
      static constexpr auto symbol() { return "km"; }
   };
   template <> struct io_traits<length::feet>
   {
      static constexpr auto symbol() { return "ft"; }
   };
   template <> struct io_traits<length::yard>
   {
      static constexpr auto symbol() { return "yd"; }
   };


   using meter_t = quantity_t<length::meter, double>;
   using centimeter_t = quantity_t<length::centimeter, double>;
   using kilometer_t = quantity_t<length::kilometer, double>;
   using feet_t = quantity_t<length::feet, double>;
   using yard_t = quantity_t<length::yard, double>;


   //! \todo use concept Length
   template <
      typename T1, typename T2,
      typename U1, typename U2>
   constexpr std::enable_if_t<is_same_base_unit<T1, U1>::value, quantity_t<T1, T2> >
      operator + (quantity_t<T1, T2> const& lhs, quantity_t<U1, U2> const& rhs) noexcept
   {
      return quantity_t<T1, T2>{ lhs.m_value + (rhs.m_value * T1::conversion_t::den * U1::conversion_t::num / U1::conversion_t::den) };
   }

} // namespace sec21::units

constexpr auto operator "" _m(long double val) noexcept {
   return sec21::units::meter_t{ static_cast<sec21::units::meter_t::value_t>(val) };
}

constexpr auto operator "" _cm(long double val) noexcept {
   return sec21::units::centimeter_t(static_cast<sec21::units::centimeter_t::value_t>(val));
}

constexpr auto operator "" _km(long double val) noexcept {
   return sec21::units::kilometer_t(static_cast<sec21::units::kilometer_t::value_t>(val));
}

constexpr auto operator "" _ft(long double val) noexcept {
   return sec21::units::feet_t(static_cast<sec21::units::feet_t::value_t>(val));
}

constexpr auto operator "" _yd(long double val) noexcept {
   return sec21::units::yard_t(static_cast<sec21::units::yard_t::value_t>(val));
}


//
// compiletime tests
//
static_assert(5.0_m + 5.0_m == 10.0_m);
static_assert(5.314_m + 5.314_m == 10.628_m);
static_assert(5.0_m + 500.0_cm == 10.0_m);
static_assert(5.0_m + 5.0_ft == 6.524_m);
static_assert(100.0_cm + 1.0_ft == 130.48_cm);
static_assert(100.0_cm + 0.005_km == 600.0_cm);
