#pragma once

#include <ratio>
#include <numeric>

namespace sec21::units
{
   namespace detail
   {
      template<typename R1, typename R2>
      struct common_ratio_impl {
         static constexpr auto gcd_num = std::gcd(R1::num, R2::num);
         static constexpr auto gcd_den = std::gcd(R1::den, R2::den);
         using type = std::ratio<gcd_num, (R1::den / gcd_den) * R2::den>;
      };
   }

   template <typename R1, typename R2>
   using common_ratio_t = typename detail::common_ratio_impl<R1, R2>::type;

   template<auto Num, auto Denom>
   constexpr auto factor(std::ratio<Num, Denom>) noexcept
   {
      static_assert(Denom != 0);
      return Num / Denom;
   }
}
