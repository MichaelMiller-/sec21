#pragma once

namespace sec21::units
{
   template <typename> struct io_traits;
}

#if 0

class sqauremeter;

template <typename> struct area_traits;

//! \todo dimension
template <> struct area_traits<sqauremeter> : base_traits<int>
{
   static constexpr auto symbol() {
      return "m²";
   }
};

template <typename T>
using area_value_t = typename area_traits<T>::value_type;

template <typename T>
using area = base_unit<T, area_traits>;

constexpr area<sqauremeter> operator "" _qm(long double val) {
   return { static_cast<area_value_t<sqauremeter>>(val) };
}
constexpr area<sqauremeter> operator "" _qm(unsigned long long val) {
   return { static_cast<area_value_t<sqauremeter>>(val) };
}


template <typename T>
using length_value_t = typename length_traits<T>::value_type;

template <typename T>
using length = base_unit<T, length_traits>;


class newton;
class kilonewton;
class moment;

template <typename> struct force_traits;

template <> struct force_traits<newton> : base_traits<base_dimension>
{
   static constexpr auto symbol() {
      return "N";
   }
};
template <> struct force_traits<kilonewton> : base_traits<std::kilo>
{
   static constexpr auto symbol() {
      return "kN";
   }
};
//! \todo is another traits type
template <> struct force_traits<moment> : base_traits<std::kilo>
{
   static constexpr auto symbol() {
      return "kNm";
   }
};

template <typename T>
using force_value_t = typename force_traits<T>::value_type;

template <typename T>
using force = base_unit<T, force_traits>;

#endif
