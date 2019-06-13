#pragma once
	
#ifdef pascal
   #undef pascal
#endif

#include <ratio>
#include <numeric>
#include <type_traits>

//! \todo check boost version
#include <boost/mp11.hpp>

namespace sec21::unit
{
   namespace dimension
   {
      // International System of Units
      // clang-format off
      template <int Length, int Mass, int Time, int ElectricCurrent, int Temperature>
      using dimension_t = boost::mp11::mp_list_c<int, Length, Mass, Time, ElectricCurrent, Temperature>;

      using scalar         = dimension_t<0, 0, 0, 0, 0>;

      using length         = dimension_t<1, 0, 0, 0, 0>;
      using mass           = dimension_t<0, 1, 0, 0, 0>;
      using time           = dimension_t<0, 0, 1, 0, 0>;
      using charge         = dimension_t<0, 0, 0, 1, 0>;
      using temperature    = dimension_t<0, 0, 0, 0, 1>;

      using area           = dimension_t<2, 0, 0, 0, 0>;
      using volume         = dimension_t<3, 0, 0, 0, 0>;

      using velocity       = dimension_t<1, 0, -1, 0, 0>;
      using acceleration   = dimension_t<1, 0, -2, 0, 0>;

      using force          = dimension_t<1, 1, -2, 0, 0>;
      using torque         = dimension_t<2, 1, -2, 0, 0>;

      using pressure       = dimension_t<-1, 1, -2, 0, 0>;
      // clang-format on
   }

   namespace detail
   {
      template<typename ScaleFrom, typename ScaleTo>
      struct scale_helper
      {
         using div_t = typename std::ratio_divide<ScaleFrom, ScaleTo>::type;

         template <typename T>
         constexpr static T apply(T const& t) noexcept(noexcept((div_t::num * t) / div_t::den))
         {
            return (div_t::num * t) / div_t::den;
         }
      };

      template <typename R1, typename R2>
      using common_ratio = std::ratio<std::gcd(R1::num, R2::num), std::lcm(R1::den, R2::den)>;

      template <typename T, typename U>
      using plus_f = boost::mp11::mp_int<T::value + U::value>;

      template <typename T, typename U>
      using minus_f = boost::mp11::mp_int<T::value - U::value>;
   }

   template <typename T, typename Dimension, typename Scale>
   class quantity
   {
      T m_value;

      static_assert(std::is_arithmetic_v<T>, "T must be an arithmetic type");

   public:
      using value_t = T;
      using dimension_t = Dimension;
      using scale_t = Scale;

      template <typename U
#ifndef __cpp_concepts
      , typename = std::enable_if_t<std::is_constructible_v<value_t, U>>>
#else   
      > requires std::is_constructible_v<value_t, U>
#endif
      constexpr quantity(U && u) noexcept
         : m_value{ std::forward<U>(u) }
      {}

      template <typename U, typename S>
      constexpr quantity(quantity<U, Dimension, S> const& other) noexcept
         : m_value{ detail::scale_helper<S, Scale>::apply(static_cast<T>(other.get())) }
      {}

      constexpr auto get() const noexcept { return m_value; }

      template <typename U, typename S>
      friend constexpr bool operator == (quantity const& lhs, quantity<U, Dimension, S> const& rhs) noexcept {
         return lhs.m_value == quantity(rhs).m_value;
      }

      template <typename U, typename S>
      friend constexpr bool operator != (quantity const& lhs, quantity<U, Dimension, S> const& rhs) noexcept {
         return !(lhs == rhs);
      }

      template <typename U, typename S>
      friend constexpr auto operator + (quantity const& lhs, quantity<U, Dimension, S> const& rhs) noexcept
      {
         using result_ratio_t = detail::common_ratio<Scale, S>;
         using result_t = quantity<decltype(lhs.get() + rhs.get()), Dimension, result_ratio_t>;

         return result_t{ result_t(lhs).get() + result_t(rhs).get() };
      }

      template <typename U
#ifndef __cpp_concepts
      , typename = std::enable_if_t<std::is_convertible_v<U, value_t>>>
#else   
         //! \todo is_nothrow_convertible
      > requires std::is_convertible_v<U, value_t>
#endif
      friend constexpr auto operator + (quantity const& lhs, U const& rhs) noexcept
      {
         return quantity{ lhs.get() + rhs };
      }

      template <typename U, typename S>
      friend constexpr auto operator - (quantity const& lhs, quantity<U, Dimension, S> const& rhs) noexcept
      {
         using result_ratio_t = detail::common_ratio<Scale, S>;
         using result_t = quantity<decltype(lhs.get() - rhs.get()), Dimension, result_ratio_t>;

         return result_t{ result_t(lhs).get() - result_t(rhs).get() };
      }

      template <typename U
#ifndef __cpp_concepts
      , typename = std::enable_if_t<std::is_convertible_v<U, value_t>>>
#else   
         //! \todo is_nothrow_convertible
      > requires std::is_convertible_v<U, value_t>
#endif
      friend constexpr auto operator - (quantity const& lhs, U const& rhs) noexcept
      {
         return quantity{ lhs.get() - rhs };
      }

      template <typename U, typename D, typename S>
      friend constexpr auto operator * (quantity const& lhs, quantity<U, D, S> const& rhs) noexcept
      {
         using result_ratio_t = std::ratio_multiply<Scale, S>;
         using result_dim_t = boost::mp11::mp_transform<detail::plus_f, Dimension, D>;
         using result_t = quantity<decltype(lhs.get() * rhs.get()), result_dim_t, result_ratio_t>;

         return result_t{ lhs.get() * rhs.get() };
      }

      template <typename U
#ifndef __cpp_concepts
      , typename = std::enable_if_t<std::is_convertible_v<U, value_t>>>
#else   
         //! \todo is_nothrow_convertible
      > requires std::is_convertible_v<U, value_t>
#endif
      friend constexpr auto operator * (quantity const& lhs, U const& rhs) noexcept
      {
         return quantity{ lhs.get() * rhs };
      }      

      template <typename U, typename D, typename S>
      friend constexpr auto operator / (quantity const& lhs, quantity<U, D, S> const& rhs) noexcept
      {
         using result_ratio_t = std::ratio_multiply<Scale, S>;
         using result_dim_t = boost::mp11::mp_transform<detail::minus_f, Dimension, D>;
         using result_t = quantity<decltype(lhs.get() * rhs.get()), result_dim_t, result_ratio_t>;

         return result_t{ lhs.get() * rhs.get() };
      }

      template <typename U
#ifndef __cpp_concepts
      , typename = std::enable_if_t<std::is_convertible_v<U, value_t>>>
#else   
         //! \todo is_nothrow_convertible
      > requires std::is_convertible_v<U, value_t>
#endif
      friend constexpr auto operator / (quantity const& lhs, U const& rhs) noexcept
      {
         return quantity{ lhs.get() / rhs };
      }      

      constexpr const auto operator - () const noexcept {
         auto new_value = -get();
         return quantity(std::move(new_value));
      }
   };

   // clang-format off
   //
   // quantities
   //
   template <typename T> using id_t = quantity<T, dimension::scalar, std::ratio<1>>;
   template <typename T> using scalar_t = quantity<T, dimension::scalar, std::ratio<1>>;

   template <typename T, typename Scale> using length_t = quantity<T, dimension::length, Scale>;
   template <typename T, typename Scale> using mass_t = quantity<T, dimension::mass, Scale>;
   template <typename T, typename Scale> using time_t = quantity<T, dimension::time, Scale>;
   template <typename T, typename Scale> using charge_t = quantity<T, dimension::charge, Scale>;
   template <typename T, typename Scale> using temperature_t = quantity<T, dimension::temperature, Scale>;

   template <typename T, typename Scale> using area_t = quantity<T, dimension::area, Scale>;
   template <typename T, typename Scale> using volume_t = quantity<T, dimension::volume, Scale>;

   template <typename T, typename Scale> using velocity_t = quantity<T, dimension::velocity, Scale>;
   template <typename T, typename Scale> using acceleration_t = quantity<T, dimension::acceleration, Scale>;

   template <typename T, typename Scale> using force_t = quantity<T, dimension::force, Scale>;
   template <typename T, typename Scale> using torque_t = quantity<T, dimension::torque, Scale>;

   template <typename T, typename Scale> using pressure_t = quantity<T, dimension::pressure, Scale>;
   //
   // scaled and derived units
   //
   // length
   template <typename T> using millimeter = length_t<T, std::milli>;
   template <typename T> using centimeter = length_t<T, std::centi>;
   template <typename T> using meter = length_t<T, std::ratio<1>>;         // SI-Unit
   template <typename T> using kilometer = length_t<T, std::kilo>;

   template <typename T> using inch = length_t<T, std::ratio<254, 10'000>::type>;
   template <typename T> using foot = length_t<T, std::ratio<3048, 10'000>::type>;
   template <typename T> using yard = length_t<T, std::ratio<9144, 10'000>::type>;
   template <typename T> using mile = length_t<T, std::ratio<1609344, 1'000>::type>;

   // mass
   template <typename T> using gram = mass_t<T, std::milli>;
   template <typename T> using kilogram = mass_t<T, std::ratio<1>>;       // SI-Unit
   template <typename T> using ton = mass_t<T, std::kilo>;

   template <typename T> using pound = mass_t<T, std::ratio<4535924, 10'000'000>::type>;

   // time
   template <typename T> using second = time_t<T, std::ratio<1>>;         // SI-Unit
   template <typename T> using minute = time_t<T, std::ratio<60>>;
   template <typename T> using hour = time_t<T, std::ratio<3600>>;

   // charge
   template <typename T> using milliampere = charge_t<T, std::milli>;
   template <typename T> using ampere = charge_t<T, std::ratio<1>>;        // SI-Unit

   // temperature
   template <typename T> using kelvin = temperature_t<T, std::ratio<1>>;   // SI-Unit

   // area
   template <typename T> using square_millimeter = area_t<T, std::micro>;  
   template <typename T> using square_meter = area_t<T, std::ratio<1>>;
   template <typename T> using square_foot = area_t<T, std::ratio<145161, 1562500>>;

   // volumen
   template <typename T> using cubic_meter = volume_t<T, std::ratio<1>>;

   // velocity
   template <typename T> using kmh = velocity_t<T, std::ratio<1000, 3600>::type>;
   template <typename T> using mph = velocity_t<T, std::ratio<1609344, 3600000>::type>;

   // force
   template <typename T> using newton = force_t<T, std::ratio<1>>;         // Base-Unit
   template <typename T> using kilonewton = force_t<T, std::kilo>;
   template <typename T> using meganewton = force_t<T, std::mega>;

   // torque
   template <typename T> using newton_meter = torque_t<T, std::ratio<1>>;  // Base-Unit
   template <typename T> using kilonewton_meter = torque_t<T, std::kilo>;

   // pressure
   template <typename T> using pascal = pressure_t<T, std::ratio<1>>;      // Base-Unit [N m^-2] [kg m^-1 s^-2]
   template <typename T> using kilopascal = pressure_t<T, std::kilo>;      // 1 kN/m^2 == 1kPa
   template <typename T> using megapascal = pressure_t<T, std::mega>;      // 1 N/mm^2 == 1MPa
   template <typename T> using gigapascal = pressure_t<T, std::giga>;

   //template <typename T> using youngs_modulus = pressure_t<T, std::ratio<1>>;      // Base-Unit [kg m^-1 s^-2]
   
   inline namespace literals
   {
      // length
      //
      constexpr auto operator "" _mm(unsigned long long v) noexcept  -> millimeter<unsigned long long> { return v; }
      constexpr auto operator "" _mm(long double v) noexcept         -> millimeter<double> { return static_cast<double>(v); }

      constexpr auto operator "" _cm(unsigned long long v) noexcept  -> centimeter<unsigned long long> { return v; }
      constexpr auto operator "" _cm(long double v) noexcept         -> centimeter<double> { return static_cast<double>(v); }

      constexpr auto operator "" _m(unsigned long long v) noexcept   -> meter<unsigned long long> { return v; }
      constexpr auto operator "" _m(long double v) noexcept          -> meter<double> { return static_cast<double>(v); }

      constexpr auto operator "" _km(unsigned long long v) noexcept  -> kilometer<unsigned long long> { return v; }
      constexpr auto operator "" _km(long double v) noexcept         -> kilometer<double> { return static_cast<double>(v); }

      constexpr auto operator "" _in(unsigned long long v) noexcept  -> inch<unsigned long long> { return v; }
      constexpr auto operator "" _in(long double v) noexcept         -> inch<double> { return static_cast<double>(v); }

      constexpr auto operator "" _ft(unsigned long long v) noexcept  -> foot<unsigned long long> { return v; }
      constexpr auto operator "" _ft(long double v) noexcept         -> foot<double> { return static_cast<double>(v); }

      constexpr auto operator "" _yd(unsigned long long v) noexcept  -> yard<unsigned long long> { return v; }
      constexpr auto operator "" _yd(long double v) noexcept         -> yard<double> { return static_cast<double>(v); }

      // mass
      //
      constexpr auto operator "" _g(unsigned long long v) noexcept   -> gram<unsigned long long> { return v; }
      constexpr auto operator "" _g(long double v) noexcept          -> gram<double> { return static_cast<double>(v); }

      constexpr auto operator "" _kg(unsigned long long v) noexcept  -> kilogram<unsigned long long> { return v; }
      constexpr auto operator "" _kg(long double v) noexcept         -> kilogram<double> { return static_cast<double>(v); }

      constexpr auto operator "" _t(unsigned long long v) noexcept   -> ton<unsigned long long> { return v; }
      constexpr auto operator "" _t(long double v) noexcept          -> ton<double> { return static_cast<double>(v); }

      constexpr auto operator "" _lb(unsigned long long v) noexcept   -> pound<unsigned long long> { return v; }
      constexpr auto operator "" _lb(long double v) noexcept          -> pound<double> { return static_cast<double>(v); }

      // time
      //
      constexpr auto operator "" _s(unsigned long long v) noexcept   -> second<unsigned long long> { return v; }
      constexpr auto operator "" _s(long double v) noexcept          -> second<double> { return static_cast<double>(v); }

      constexpr auto operator "" _min(unsigned long long v) noexcept -> minute<unsigned long long> { return v; }
      constexpr auto operator "" _min(long double v) noexcept        -> minute<double> { return static_cast<double>(v); }

      constexpr auto operator "" _h(unsigned long long v) noexcept   -> hour<unsigned long long> { return v; }
      constexpr auto operator "" _h(long double v) noexcept          -> hour<double> { return static_cast<double>(v); }

      // area
      //
      //! \todo 

      // force
      //
      constexpr auto operator "" _N(long double v) noexcept  -> newton<double> { return static_cast<double>(v); }
      constexpr auto operator "" _kN(long double v) noexcept -> kilonewton<double> { return static_cast<double>(v); }
      constexpr auto operator "" _MN(long double v) noexcept -> meganewton<double> { return static_cast<double>(v); }

      // torque
      //
      constexpr auto operator "" _Nm(long double v) noexcept  -> newton_meter<double> { return static_cast<double>(v); }
      constexpr auto operator "" _kNm(long double v) noexcept -> kilonewton_meter<double> { return static_cast<double>(v); }

      // pressure
      //
      constexpr auto operator "" _Pa(long double v) noexcept  -> pascal<double> { return static_cast<double>(v); }
      constexpr auto operator "" _kPa(long double v) noexcept -> kilopascal<double> { return static_cast<double>(v); }
      constexpr auto operator "" _MPa(long double v) noexcept -> megapascal<double> { return static_cast<double>(v); }
      constexpr auto operator "" _GPa(long double v) noexcept -> gigapascal<double> { return static_cast<double>(v); }
   }
   // clang-format on

   inline namespace constants
   {
      constexpr auto g = acceleration_t<double, std::ratio<1>>{ 9.80665 };

      //! \todo 
      // Young's modulus E = [ kg m-1 s-2 ]
      //
      constexpr auto baustahl = 210.0_GPa;
      constexpr auto titan = 110.0_GPa;
      constexpr auto gold = 78.0_GPa;
      constexpr auto blei = 19.0_GPa;
   }
}
  
#ifdef __cpp_concepts
namespace sec21 
{
   namespace detail 
   {
      template <typename T> using value_t = typename T::value_t;
      template <typename T> using dimension_t = typename T::dimension_t;
      template <typename T> using scale_t = typename T::scale_t;
   }

   template <typename T>
   concept bool Unit = requires(T t) { 
      typename detail::value_t<T>;
      typename detail::dimension_t<T>;
      typename detail::scale_t<T>;
   };

   template <typename T>
   concept bool Length = Unit<T> && std::is_same_v<detail::dimension_t<T>, unit::dimension::length>;

   template <typename T>
   concept bool Mass = Unit<T> && std::is_same_v<detail::dimension_t<T>, unit::dimension::mass>;

   template <typename T>
   concept bool Time = Unit<T> && std::is_same_v<detail::dimension_t<T>, unit::dimension::time>;

   template <typename T>
   concept bool Charge = Unit<T> && std::is_same_v<detail::dimension_t<T>, unit::dimension::charge>;

   template <typename T>
   concept bool Temperature = Unit<T> && std::is_same_v<detail::dimension_t<T>, unit::dimension::temperature>;


   template <typename T>
   concept bool Area = Unit<T> && std::is_same_v<detail::dimension_t<T>, unit::dimension::area>;

   template <typename T>
   concept bool Volume = Unit<T> && std::is_same_v<detail::dimension_t<T>, unit::dimension::volume>;


   template <typename T>
   concept bool Velocity = Unit<T> && std::is_same_v<detail::dimension_t<T>, unit::dimension::velocity>;

   template <typename T>
   concept bool Acceleration = Unit<T> && std::is_same_v<detail::dimension_t<T>, unit::dimension::acceleration>;


   template <typename T>
   concept bool Force = Unit<T> && std::is_same_v<detail::dimension_t<T>, unit::dimension::force>;

   template <typename T>
   concept bool Torque = Unit<T> && std::is_same_v<detail::dimension_t<T>, unit::dimension::torque>;   
}
#endif
