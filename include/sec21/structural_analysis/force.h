#pragma once

#include <sec21/units.h>

#include <array>
#include <algorithm>

namespace sec21::structural_analysis
{
   template <std::size_t Dimension, typename Unit = unit::kilonewton<double>>
#ifdef __cpp_concepts
   requires Force<Unit>
#endif
   class force
   {
      static_assert(Dimension == 2 || Dimension == 3, "Only works in 2D and 3D");

      std::array<Unit, Dimension> value;

   public:
      template <typename... Args>
      constexpr force(Args &&... args) noexcept 
         : value{ std::forward<Args>(args)... } 
      {
         static_assert(sizeof...(args) < Dimension + 1, "too many arguments for this class");
      }

      constexpr auto begin() const noexcept
      {
         return std::begin(value); 
      }
      constexpr auto end() const noexcept 
      { 
         return std::end(value); 
      }

      [[nodiscard]] friend constexpr auto operator + (force<Dimension> const& lhs, force<Dimension> const& rhs) noexcept -> force<Dimension>
      {
         if constexpr (Dimension == 2) {
            return { 
               std::get<0>(lhs.value) + std::get<0>(rhs.value),
               std::get<1>(lhs.value) + std::get<1>(rhs.value)
            };
         }
         if constexpr (Dimension == 3) {
            return { 
               std::get<0>(lhs.value) + std::get<0>(rhs.value),
               std::get<1>(lhs.value) + std::get<1>(rhs.value),
               std::get<2>(lhs.value) + std::get<2>(rhs.value)
            };
         }
      }

#ifdef __cpp_lib_constexpr_misc
      [[nodiscard]] friend constexpr bool operator == (force<Dimension> const& lhs, force<Dimension> const& rhs) noexcept {
         return std::equal(std::begin(lhs.value), std::end(lhs.value), std::begin(rhs.value));
      }
      [[nodiscard]] friend constexpr bool operator != (force<Dimension> const& lhs, force<Dimension> const& rhs) noexcept {
         return !(lhs == rhs);
      }
#else
      [[nodiscard]] friend bool operator == (force<Dimension> const& lhs, force<Dimension> const& rhs) noexcept {
         return std::equal(std::begin(lhs.value), std::end(lhs.value), std::begin(rhs.value));
      }
      [[nodiscard]] friend bool operator != (force<Dimension> const& lhs, force<Dimension> const& rhs) noexcept {
         return !(lhs == rhs);
      }
#endif
   };

   template <typename Iterator>
   [[nodiscard]] auto superposition(Iterator first, Iterator last) noexcept
   {
      std::decay_t<std::iterator_traits<Iterator>::value_type> result{};
      //! \todo 2019-04-26 basically the same as std::accumulate, but the code below didn't work 
      // return std::accumulate(first, last, std::decay_t<std::iterator_traits<Iterator>::value_type>{});
      for (; first != last; ++first)
         result = result + *first;
      return result;
   }
}
