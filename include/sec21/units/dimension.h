#pragma once

#include <type_traits>

#include <sec21/units/tags.h>
#include <sec21/units/traits.h>

namespace sec21::units
{
   template <typename Conversion, typename BaseUnit>
   struct dimension_t : detail::dimension_tag
   {
      using conversion_t = Conversion;
      using base_unit_t = BaseUnit;

      //typedef typename units::traits::unit_traits<BaseUnit>::base_unit_type base_unit_type;
      //typedef typename std::ratio_multiply<typename BaseUnit::conversion_ratio, Conversion> conversion_ratio;
      //typedef typename std::ratio_add<typename BaseUnit::pi_exponent_ratio, PiExponent> pi_exponent_ratio;
      //typedef typename std::ratio_add<std::ratio_multiply<typename BaseUnit::conversion_ratio, Translation>, typename BaseUnit::translation_ratio> translation_ratio;
   };

   template <typename T, class = typename std::enable_if<is_dimension<T>::value>::type>
   constexpr auto quotient(T && dim) noexcept {
      return  T::conversion_t::num / T::conversion_t::den;
   }
}
