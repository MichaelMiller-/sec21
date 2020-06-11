#pragma once

#include <type_traits>
#include <limits>
#include <ratio>

#include <sec21/units/concepts.h>
#include <sec21/units/dimension.h>
#include <sec21/units/unit.h>
#include <sec21/units/ratio.h>

namespace sec21::units
{
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

      template <Scalar U> // requires std::is_nothrow_convertible<U, T>
      constexpr quantity(U u) noexcept
         : m_value{ static_cast<T>(u) }
      {}

      template <Quantity Q>
      requires SameDimension<quantity<Unit, T>, Q>
      constexpr quantity(Q const& q) noexcept
         : m_value{ quantity_cast<quantity>(q).value() }
      {}

      constexpr auto value() const noexcept {
         return m_value;
      }

      constexpr auto operator - () const noexcept {
         return quantity(-value());
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

   template <Quantity Q1, Quantity Q2> requires SameDimension<Q1, Q2>
   constexpr bool operator == (Q1 const& lhs, Q2 const& rhs) noexcept
   {
      using ct = common_quantity_t<Q1, Q2>;
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

   template <Quantity Q1, Quantity Q2> requires SameDimension<Q1, Q2>
   constexpr auto operator * (Q1 const& lhs, Q2 const& rhs) 
   {
      using vt = decltype(lhs.value() * rhs.value());
      using unit1_t = typename Q1::unit_t;
      using unit2_t = typename Q2::unit_t;
      using ratio1_t = typename unit1_t::ratio_t;
      using ratio2_t = typename unit2_t::ratio_t;
      using dt = multiply_dimension_t<typename Q1::dimension_t, typename Q2::dimension_t>;
      using rt = quantity<unit<dt, std::ratio<1>>, vt>;

      const auto v1 = lhs.value() * ratio1_t::num;
      const auto v2 = Q1{ rhs.value() * ratio2_t::num }.value();
      return rt{ v1 * v2 };
   }

   template <Quantity Q1, Quantity Q2>
   constexpr auto operator * (Q1 const& lhs, Q2 const& rhs) 
   {
      using vt = decltype(lhs.value() * rhs.value());
      using dt = multiply_dimension_t<typename Q1::dimension_t, typename Q2::dimension_t>;
      using rt = quantity<unit<dt, std::ratio<1>>, vt>;

      return rt{ lhs.value() * rhs.value() };
   }

   template <Quantity Q1, Quantity Q2> requires SameDimension<Q1, Q2>
   constexpr auto operator / (Q1 const& lhs, Q2 const& rhs)
   {
      // [[expects: rhs != 0]]
      return lhs.value() / Q1{ rhs }.value();
   }

   template <Quantity Q1, Quantity Q2>
   [[nodiscard]] constexpr auto operator / (Q1 const& lhs, Q2 const& rhs)
   {
      // [[expects: rhs != 0]]
      using vt = decltype(lhs.value() / rhs.value());
      using dt = divide_dimension_t<typename Q1::dimension_t, typename Q2::dimension_t>;
      using unit1_t = typename Q1::unit_t;
      using unit2_t = typename Q2::unit_t;
      using ratio1_t = typename unit1_t::ratio_t;
      using ratio2_t = typename unit2_t::ratio_t;
      using rt = quantity<unit<dt, std::ratio_divide<ratio1_t, ratio2_t>>, vt>;

      return rt{ lhs.value() / rhs.value() };
   }

   template <Quantity Q, Scalar T>
   [[nodiscard]] constexpr auto operator / (Q const& lhs, T rhs) -> Q
   {
      // [[expects: rhs != 0]]
      return { lhs.value() / rhs };
   }

   template <Scalar T, Quantity Q>
   [[nodiscard]] constexpr auto operator / (T lhs, Q const& rhs)
   {
      // [[expects: rhs != quantity<U2, T2>(0)]]
      using vt = decltype(lhs / rhs.value());
      using dt = invert_dimension_t<typename Q::dimension_t>;
      using ut = typename Q::unit_t;
      using ratio_t = typename ut::ratio_t;
      using rt = quantity<unit<dt, std::ratio<1>>, vt>;

      return rt{ lhs / rhs.value() *  ratio_t::den / ratio_t::num };
   }

   template <typename T>
   struct type_info {};

   template <typename T>
   struct abbreviation {};

   template <typename CharT, typename Trais, typename Unit, typename T>
   auto operator << (std::basic_ostream<CharT, Trais>& os, quantity<Unit, T> const& v) -> std::basic_ostream<CharT, Trais>&
   {
      return os << v.value(); // << abbreviation<T>::value;
   }
}


#include <nlohmann/json.hpp>
#include <boost/lexical_cast.hpp>
#include <tuple>
#include <sstream>
#include <string_view>

namespace sec21::units
{
   namespace detail
   {
      template <typename T>
      auto to_abbreviation(T) noexcept
      {
         return abbreviation<T>::value;
      }
      template <typename Tuple, std::size_t... Is>
      constexpr decltype(auto) transform_to_abbreviation(Tuple&& tuple, std::index_sequence<Is...>)
      {
         // c++20
         // constexpr auto impl = []<typename T>(T){ return abbreviation<T>::value; };
         return std::tuple{ to_abbreviation(std::get<Is>(tuple))...};
      }

      template <typename T, typename Precision = double>
      auto valid_dimension(std::string_view unit) -> bool
      {
         typename type_info<T>::valid_types_t valid_types{};

         constexpr auto N = std::tuple_size_v<decltype(valid_types)>;
         const auto dfs = transform_to_abbreviation(valid_types, std::make_index_sequence<N>{});

         return std::apply([&](auto const &... v) { return ((v == unit) || ...); }, dfs);
      }

      auto split(std::string const& input)
      {
         const std::string delimiter = "_";    
         auto pos = input.find(delimiter);
         if (pos == std::string::npos)
            throw std::runtime_error("");

         auto token = input.substr(0, pos);
         auto unit = input.substr(pos + std::size(delimiter));

         return std::tuple{token, unit};
      }

      template <typename Unit, typename T>
      auto to_string(quantity<Unit, T> const& obj) -> std::string
      {
         std::stringstream ss;
         ss << obj.value() << '_' << abbreviation<Unit>::value;
         return ss.str();
      }

      template <typename Unit, typename T>
      auto from_string(std::string const& input) -> quantity<Unit, T>
      {
         auto [value, unit] = split(input);

         if (bool valid = valid_dimension<typename Unit::dimension_t>(unit); valid == false)
            throw std::runtime_error("couldn't match input unit with dimension type");

         auto v = boost::lexical_cast<double>(value) * 1.0;
         //! \todo 
         return { static_cast<T>(v) };
      }
   }

   template <typename Unit, typename T>
   void to_json(nlohmann::json& j, quantity<Unit, T> const& obj) {
      // auto v = obj.value();
      j = nlohmann::json{
         {"value", detail::to_string(obj)}
      };
   }

   template <typename Unit, typename T>
   void from_json(nlohmann::json const& j, quantity<Unit, T>& obj) 
   {
      std::string value;
      j.at("value").get_to(value);
      obj = detail::from_string<Unit, T>(value);
   }
}