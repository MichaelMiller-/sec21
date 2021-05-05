#pragma once

namespace sec21::policy
{
   template <typename T, typename U>
   struct mixed_addition
   {
      friend constexpr T& operator += (T& lhs, U const& rhs) noexcept
      {
         using lhs_t = typename T::underlying_t;
         using rhs_t = typename U::underlying_t;
         static_assert(std::is_same_v<lhs_t, rhs_t>);

         static_cast<lhs_t&>(lhs) += static_cast<rhs_t const&>(rhs);
         return lhs;
      }
      friend constexpr T operator + (T const& lhs, U const& rhs) noexcept
      {
         using lhs_t = typename T::underlying_t;
         using rhs_t = typename U::underlying_t;
         static_assert(std::is_same_v<lhs_t, rhs_t>);

         return T(static_cast<lhs_t const&>(lhs) + static_cast<rhs_t const&>(rhs));
      }
   };

   template <typename T>
   using addition = mixed_addition<T, T>;
}
