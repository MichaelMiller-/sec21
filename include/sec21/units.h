#pragma once

#include <ratio>
#include <numeric>

#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/minus.hpp>

namespace sec21::unit
{
   namespace dimension
   {
      // International System of Units
      //! \todo consider other dimensions
      //! \todo not a good wrapper; maybe a another container or boost::hana
      template <int Length, int Mass, int Time, int ElectricCurrent, int Temperature>
      using dimension_t = boost::mpl::vector5<
         boost::mpl::integral_c<decltype(Length), Length>,
         boost::mpl::integral_c<decltype(Mass), Mass>,
         boost::mpl::integral_c<decltype(Time), Time>,
         boost::mpl::integral_c<decltype(ElectricCurrent), ElectricCurrent>,
         boost::mpl::integral_c<decltype(Temperature), Temperature>
      >;

      // clang-format off
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

      //! \todo replace with "using"
      template <typename R1, typename R2>
      struct common_ratio
      {
         using type = typename std::ratio<std::gcd(R1::num, R2::num), std::lcm(R1::den, R2::den)>::type;
      };

      struct plus_f
      {
         template <typename T, typename U>
         struct apply
         {
            using type = typename boost::mpl::plus<T, U>::type;
         };
      };

      struct minus_f
      {
         template <typename T, typename U>
         struct apply
         {
            using type = typename boost::mpl::minus<T, U>::type;
         };
      };
   }

   template <typename T, typename Dimension, typename Scale>
   class quantity
   {
      T m_value;

   public:
      using value_t = T;
      using dimension_t = Dimension;
      using scale_t = Scale;

      //! \todo check noexcept
      // explicit
      constexpr quantity(T && t) noexcept // (noexcept(T{ std::move(t) }))
         : m_value{ std::forward<T>(t) }
      {}

      template <typename U, typename S>
      //! \todo check noexcept
      constexpr quantity(quantity<U, Dimension, S> const& other) noexcept
         : m_value{ detail::scale_helper<S, Scale>::apply(static_cast<T>(other.get())) }
      {}

      constexpr const T& get() const noexcept { return m_value; }

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
         using result_ratio_t = typename detail::common_ratio<Scale, S>::type;
         using result_t = quantity<decltype(lhs.get() + rhs.get()), Dimension, result_ratio_t>;

         return result_t{ result_t(lhs).get() + result_t(rhs).get() };
      }

      template <typename U, typename S>
      friend constexpr auto operator - (quantity const& lhs, quantity<U, Dimension, S> const& rhs) noexcept
      {
         using result_ratio_t = typename detail::common_ratio<Scale, S>::type;
         using result_t = quantity<decltype(lhs.get() - rhs.get()), Dimension, result_ratio_t>;

         return result_t{ result_t(lhs).get() - result_t(rhs).get() };
      }

      template <typename U, typename D, typename S>
      friend constexpr auto operator * (quantity const& lhs, quantity<U, D, S> const& rhs) noexcept
      {
         using result_ratio_t = typename std::ratio_multiply<Scale, S>::type;
         using result_dim_t = typename boost::mpl::transform<Dimension, D, detail::plus_f>::type;
         using result_t = quantity<decltype(lhs.get() * rhs.get()), result_dim_t, result_ratio_t>;

         return result_t{ lhs.get() * rhs.get() };
      }

      template <typename U, typename D, typename S>
      friend constexpr auto operator / (quantity const& lhs, quantity<U, D, S> const& rhs) noexcept
      {
         using result_ratio_t = typename std::ratio_multiply<Scale, S>::type;
         using result_dim_t = typename boost::mpl::transform<Dimension, D, detail::minus_f>::type;
         using result_t = quantity<decltype(lhs.get() * rhs.get()), result_dim_t, result_ratio_t>;

         return result_t{ lhs.get() * rhs.get() };
      }

      constexpr const auto operator - () const noexcept {
         auto new_value = -get();
         return quantity(std::move(new_value));
      }
   };

#ifdef __cpp_deduction_guides
   //! \todo 
#endif

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


   inline namespace literals
   {
      // length
      //
      constexpr auto operator "" _mm(unsigned long long v) noexcept  -> millimeter<long long> { return std::move(v); }
      //! \todo solve long double to double conversion 
      constexpr auto operator "" _mm(long double v) noexcept         -> millimeter<double> { return std::move(v); }

      constexpr auto operator "" _cm(unsigned long long v) noexcept  -> centimeter<long long> { return std::move(v); }
      constexpr auto operator "" _cm(long double v) noexcept         -> centimeter<double> { return std::move(v); }

      constexpr auto operator "" _m(unsigned long long v) noexcept   -> meter<long long> { return std::move(v); }
      constexpr auto operator "" _m(long double v) noexcept          -> meter<double> { return std::move(v); }

      constexpr auto operator "" _km(unsigned long long v) noexcept  -> kilometer<long long> { return std::move(v); }
      constexpr auto operator "" _km(long double v) noexcept         -> kilometer<double> { return std::move(v); }

      constexpr auto operator "" _in(unsigned long long v) noexcept  -> inch<long long> { return std::move(v); }
      constexpr auto operator "" _in(long double v) noexcept         -> inch<double> { return std::move(v); }

      constexpr auto operator "" _ft(unsigned long long v) noexcept  -> foot<long long> { return std::move(v); }
      constexpr auto operator "" _ft(long double v) noexcept         -> foot<double> { return std::move(v); }

      constexpr auto operator "" _yd(unsigned long long v) noexcept  -> yard<long long> { return std::move(v); }
      constexpr auto operator "" _yd(long double v) noexcept         -> yard<double> { return std::move(v); }

      // mass
      //
      constexpr auto operator "" _g(unsigned long long v) noexcept   -> gram<long long> { return std::move(v); }
      constexpr auto operator "" _g(long double v) noexcept          -> gram<double> { return std::move(v); }

      constexpr auto operator "" _kg(unsigned long long v) noexcept  -> kilogram<long long> { return std::move(v); }
      constexpr auto operator "" _kg(long double v) noexcept         -> kilogram<double> { return std::move(v); }

      constexpr auto operator "" _t(unsigned long long v) noexcept   -> ton<long long> { return std::move(v); }
      constexpr auto operator "" _t(long double v) noexcept          -> ton<double> { return std::move(v); }

      constexpr auto operator "" _lb(unsigned long long v) noexcept   -> pound<long long> { return std::move(v); }
      constexpr auto operator "" _lb(long double v) noexcept          -> pound<double> { return std::move(v); }

      // temperature
      //
      constexpr auto operator "" _K(long double v) noexcept  -> kelvin<double> { return std::move(v); }

      //
      constexpr auto operator "" _N(long double v) noexcept  -> newton<double> { return std::move(v); }
      constexpr auto operator "" _kN(long double v) noexcept -> kilonewton<double> { return std::move(v); }
      constexpr auto operator "" _MN(long double v) noexcept -> meganewton<double> { return std::move(v); }

      //
      constexpr auto operator "" _Nm(long double v) noexcept  -> newton_meter<double> { return std::move(v); }
      constexpr auto operator "" _kNm(long double v) noexcept -> kilonewton_meter<double> { return std::move(v); }
   }
   // clang-format on

   inline namespace constants
   {
      constexpr auto g = acceleration_t<double, std::ratio<1>>{ 9.80665 };
   }

   //! \todo concepts
}
