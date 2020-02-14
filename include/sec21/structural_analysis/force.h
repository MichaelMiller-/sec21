#pragma once

#include <sec21/units/quantity.h>
#include <sec21/units/dimensions/force.h>

#include <array>
#include <type_traits>

namespace sec21::structural_analysis
{
   template <auto Dimension>
   using force_t = std::array<units::quantity<units::kilonewton, double>, Dimension>;

   using force_2D_t = force_t<2>;
   using force_3D_t = force_t<3>;

   template <auto Dimension>
   [[nodiscard]] constexpr auto operator + (force_t<Dimension> const& lhs, force_t<Dimension> const& rhs) noexcept -> force_t<Dimension>
   {
      if constexpr (Dimension == 2) {
         return { { std::get<0>(lhs) + std::get<0>(rhs), std::get<1>(lhs) + std::get<1>(rhs) } };
      }
      if constexpr (Dimension == 3) {
         return { { 
            std::get<0>(lhs) + std::get<0>(rhs),
            std::get<1>(lhs) + std::get<1>(rhs),
            std::get<2>(lhs) + std::get<2>(rhs)
         } };
      }
   }

   template <typename Iterator>
   [[nodiscard]] auto superposition(Iterator first, Iterator last) noexcept
   {
      typename std::iterator_traits<Iterator>::value_type result{};

      for (auto it = first; it != last; ++it)
         result = result + *it;
      //! \todo 2019-08-08: is not working: return std::reduce(first, last);
      return result;
   }
}
