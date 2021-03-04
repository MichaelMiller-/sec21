#pragma once

#include <sec21/units/concepts.h>
#include <sec21/units/dimension.h>
#include <sec21/units/unit.h>
#include <sec21/units/ratio.h>

#include <sstream>
#include <string>
#include <type_traits>
#include <limits>
#include <ratio>

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

#ifdef __cpp_concepts
      template <Scalar U>
#else
      template <typename U, std::enable_if_t<std::is_scalar<U>::value, bool> = true>
#endif
      constexpr quantity(U u) 
#ifdef __cpp_lib_is_nothrow_convertible      
      noexcept(std::is_nothrow_convertible<U, T>::value)
#endif      
         : m_value{ static_cast<T>(u) }
      {}

#ifdef __cpp_concepts
      template <Quantity Q> requires SameDimension<quantity<Unit, T>, Q>
#else
      template <typename Q, std::enable_if_t<has_same_dimension<quantity<Unit, T>, Q>::value, bool> = true>
#endif
      constexpr quantity(Q const& q) noexcept
         : m_value{ quantity_cast<quantity>(q).value() }
      {}

      [[nodiscard]] constexpr auto value() const noexcept {
         return m_value;
      }

      [[nodiscard]] constexpr auto operator - () const noexcept {
         return quantity(-value());
      }

      [[nodiscard]] static constexpr quantity min() noexcept
      {
         return std::numeric_limits<value_t>::lowest();
      }
      [[nodiscard]] static constexpr quantity max() noexcept
      {
         return std::numeric_limits<value_t>::max();
      }

      friend std::ostream& operator << (std::ostream& os, quantity<Unit, T> const& v)
      {
         return os << v.value(); // to_string(v);
      }      
   };

#ifdef __cpp_concepts
   template <Quantity Q1, Quantity Q2> requires SameDimension<Q1, Q2>
#else
   template <
      typename Q1, 
      typename Q2, 
      std::enable_if_t<has_same_dimension<Q1, Q2>::value, bool> = true>
#endif
   constexpr bool operator == (Q1 const& lhs, Q2 const& rhs) noexcept
   {
      return lhs.value() == Q1{ rhs }.value();
   }

#ifdef __cpp_concepts
   template <Quantity Q1, Quantity Q2> requires SameDimension<Q1, Q2>
#else
   template <
      typename Q1, 
      typename Q2, 
      std::enable_if_t<has_same_dimension<Q1, Q2>::value, bool> = true>
#endif
   constexpr bool operator != (Q1 const& lhs, Q2 const& rhs) noexcept
   {
      return !(lhs == rhs);
   }

#ifdef __cpp_concepts
   template <Quantity Q1, Quantity Q2> requires SameDimension<Q1, Q2>
#else
   template <
      typename Q1, 
      typename Q2, 
      std::enable_if_t<has_same_dimension<Q1, Q2>::value, bool> = true>
#endif
   constexpr bool operator < (Q1 const& lhs, Q2 const& rhs) noexcept
   {
      return lhs.value() < Q1{ rhs }.value();
   }

#ifdef __cpp_concepts
   template <Quantity Q1, Quantity Q2> requires SameDimension<Q1, Q2>
#else
   template <
      typename Q1, 
      typename Q2, 
      std::enable_if_t<has_same_dimension<Q1, Q2>::value, bool> = true>
#endif
   constexpr bool operator <= (Q1 const& lhs, Q2 const& rhs) noexcept
   {
      return !(rhs < lhs);
   }

#ifdef __cpp_concepts
   template <Quantity Q1, Quantity Q2> requires SameDimension<Q1, Q2>
#else
   template <
      typename Q1, 
      typename Q2, 
      std::enable_if_t<has_same_dimension<Q1, Q2>::value, bool> = true>
#endif
   constexpr bool operator > (Q1 const& lhs, Q2 const& rhs) noexcept
   {
      return rhs < lhs;
   }

#ifdef __cpp_concepts
   template <Quantity Q1, Quantity Q2> requires SameDimension<Q1, Q2>
#else
   template <
      typename Q1, 
      typename Q2, 
      std::enable_if_t<has_same_dimension<Q1, Q2>::value, bool> = true>
#endif
   constexpr bool operator >= (Q1 const& lhs, Q2 const& rhs) noexcept
   {
      return !(lhs < rhs);
   }

#ifdef __cpp_concepts
   template <Quantity Q1, Quantity Q2> requires SameDimension<Q1, Q2>
#else
   template <
      typename Q1, 
      typename Q2, 
      std::enable_if_t<has_same_dimension<Q1, Q2>::value, bool> = true>
#endif
   constexpr auto operator + (Q1 const& lhs, Q2 const& rhs) noexcept
   {
      return Q1{ lhs.value() + Q1{ rhs }.value() };
   }

#ifdef __cpp_concepts
   template <Quantity Q1, Quantity Q2> requires SameDimension<Q1, Q2>
#else
   template <
      typename Q1, 
      typename Q2, 
      std::enable_if_t<has_same_dimension<Q1, Q2>::value, bool> = true>
#endif
   constexpr auto operator - (Q1 const& lhs, Q2 const& rhs) noexcept
   {
      return Q1{ lhs.value() - Q1{ rhs }.value() };
   }

#ifdef __cpp_concepts
   template <Quantity Q1, Quantity Q2> requires SameDimension<Q1, Q2>
#else
   template <
      typename Q1, 
      typename Q2, 
      std::enable_if_t<has_same_dimension<Q1, Q2>::value, bool> = true>
#endif
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

#ifdef __cpp_concepts
   template <Quantity Q1, Quantity Q2>
#else
   template <
      typename Q1, 
      typename Q2, 
      std::enable_if_t<is_quantity<Q1>::value && is_quantity<Q2>::value, bool> = true>
#endif
   constexpr auto operator * (Q1 const& lhs, Q2 const& rhs) 
   {
      using vt = decltype(lhs.value() * rhs.value());
      using dt = multiply_dimension_t<typename Q1::dimension_t, typename Q2::dimension_t>;
      using rt = quantity<unit<dt, std::ratio<1>>, vt>;

      return rt{ lhs.value() * rhs.value() };
   }

#ifdef __cpp_concepts
   template <Quantity Q1, Scalar S>
#else
   template <
      typename Q1, 
      typename S, 
      std::enable_if_t<is_quantity<Q1>::value && std::is_scalar<S>::value, bool> = true>
#endif
   constexpr auto operator * (Q1 const& lhs, S rhs) 
   {
      return Q1{ lhs.value() * rhs };
   }

#ifdef __cpp_concepts
   template <Scalar S, Quantity Q1>
#else
   template <
      typename S, 
      typename Q1, 
      std::enable_if_t<is_quantity<Q1>::value && std::is_scalar<S>::value, bool> = true>
#endif
   constexpr auto operator * (S rhs, Q1 const& lhs) 
   {
      return Q1{ lhs.value() * rhs };
   }

#ifdef __cpp_concepts
   template <Quantity Q1, Quantity Q2> requires SameDimension<Q1, Q2>
#else
   template <
      typename Q1, 
      typename Q2, 
      std::enable_if_t<has_same_dimension<Q1, Q2>::value, bool> = true>
#endif
   constexpr auto operator / (Q1 const& lhs, Q2 const& rhs)
   {
      // [[expects: rhs != 0]]
      return lhs.value() / Q1{ rhs }.value();
   }

#ifdef __cpp_concepts
   template <Quantity Q1, Quantity Q2>
#else
   template <
      typename Q1, 
      typename Q2, 
      std::enable_if_t<is_quantity<Q1>::value && is_quantity<Q2>::value, bool> = true>
#endif
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

#ifdef __cpp_concepts
   template <Quantity Q, Scalar S>
#else
   template <
      typename Q, 
      typename S, 
      std::enable_if_t<is_quantity<Q>::value && std::is_scalar<S>::value, bool> = true>
#endif
   [[nodiscard]] constexpr auto operator / (Q const& lhs, S rhs) -> Q
   {
      // [[expects: rhs != 0]]
      return { lhs.value() / rhs };
   }

#ifdef __cpp_concepts
   template <Scalar S, Quantity Q>
#else
   template <
      typename Q, 
      typename S, 
      std::enable_if_t<is_quantity<Q>::value && std::is_scalar<S>::value, bool> = true>
#endif   
   [[nodiscard]] constexpr auto operator / (S lhs, Q const& rhs)
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

#ifdef __cpp_concepts
   template <Quantity Q>
#else
   template <typename Q, std::enable_if_t<is_quantity<Q>::value, bool> = true>
#endif
   auto to_string(Q const& obj) -> std::string
   {
      std::stringstream ss;
      ss << obj.value() << '_' << abbreviation<typename Q::dimension_t>::value;
      return ss.str();
   }
}

#include <nlohmann/json.hpp>
#include <boost/lexical_cast.hpp>
#include <tuple>
#include <string_view>

namespace sec21::units
{
   namespace detail
   {
      inline auto split(std::string const& input)
      {
         //! \todo use cptr regex
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

      struct quoted_to_ratio
      {
         template <typename U>
         using fn = typename U::ratio_t;
      };

#ifdef __cpp_concepts      
      template <Quantity Q> 
#else
      template <typename Q, std::enable_if_t<is_quantity<Q>::value, bool> = true>
#endif
      auto from_string(std::string const& input) -> Q
      {
         auto [value, unit] = split(input);

         using dim_t = typename Q::dimension_t;
         using unit_t = typename Q::unit_t;
         using R1 = typename unit_t::ratio_t;

         typename type_info<dim_t>::valid_types_t valid_types{};

         using T1 = boost::mp11::mp_transform_q<quoted_to_ratio, decltype(valid_types)>;

         constexpr auto to_abbreviation = []<typename T>(T){ return abbreviation<T>::value; };
         constexpr auto convert_ratio = []<typename T>(T){ return T::num / double{ T::den }; };

         const auto valid_abbreviations = std::apply([&](auto... n){ return std::array{ to_abbreviation(n)... }; }, valid_types);
         const auto matching_ratios = std::apply([&](auto... n){ return std::array{ convert_ratio(n)... }; }, T1{});
         static_assert(std::size(valid_abbreviations) == std::size(matching_ratios));

         const auto it = std::find(begin(valid_abbreviations), end(valid_abbreviations), unit);
         if (it == end(valid_abbreviations))
            throw std::runtime_error("Couldn't match input quantity with valid dimension type. invalid dimension: " + unit);

         const auto d = std::distance(begin(valid_abbreviations), it);
         const auto k = R1::num / double{ R1::den };
         const auto v = boost::lexical_cast<double>(value) * matching_ratios[d] / k;

         return { static_cast<typename Q::value_t>(v) };
      }    
   }

   template <typename Unit, typename T>
   void to_json(nlohmann::json& j, quantity<Unit, T> const& obj) {
      j = nlohmann::json{
         {"quantity", detail::to_string(obj)}
      };
   }

#ifdef __cpp_concepts
   template <Quantity Q>
#else
   template <typename Q, std::enable_if_t<is_quantity<Q>::value, bool> = true>
#endif
   void from_json(nlohmann::json const& j, Q& obj) 
   {
      std::string value;
      j.at("quantity").get_to(value);
      obj = detail::from_string<Q>(value);
   }
}