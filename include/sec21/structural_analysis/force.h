#pragma once

#include <sec21/units.h>

#include <array>
#include <algorithm>
#include <type_traits>

namespace sec21::structural_analysis
{
   template <std::size_t Dimension, typename Unit = unit::kilonewton<double>>
//! \todo concept
#ifdef __cpp_concepts
   requires Force<Unit>
#endif
   class force
   {
   public:
      static constexpr auto dimension_v = Dimension;
      static_assert(Dimension == 2 || Dimension == 3, "Only works in 2D and 3D");

   private:
      using value_t = std::array<Unit, Dimension>;
      value_t value;

   public:
      // template <typename... Args>
      // constexpr force(Args &&... args) noexcept 
      //    : value{ std::forward<Args>(args)... } 
      // {
      //    static_assert(sizeof...(args) < Dimension + 1, "too many arguments for this class");
      // }

      // template <typename T>
// #ifndef __cpp_concepts
//       , typename = std::enable_if_t<std::is_convertible_v<T, value_t>>>
// #else   
//       > requires std::is_constructible_v<T, value_t>
// #endif
      constexpr force(value_t && v) noexcept 
         : value{ std::move(v) } 
      {
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
            return { { 
               std::get<0>(lhs.value) + std::get<0>(rhs.value),
               std::get<1>(lhs.value) + std::get<1>(rhs.value)
            } };
         }
         if constexpr (Dimension == 3) {
            return { { 
               std::get<0>(lhs.value) + std::get<0>(rhs.value),
               std::get<1>(lhs.value) + std::get<1>(rhs.value),
               std::get<2>(lhs.value) + std::get<2>(rhs.value)
            } };
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

   namespace detail 
   {
      template <typename T>
      constexpr auto null_force() noexcept -> T
      {
         constexpr auto dim_v = T::dimension_v;

         if constexpr (dim_v == 2) {
            return { { 0.0, 0.0 } };
         }
         if constexpr (dim_v == 3) {
            return { { 0.0, 0.0, 0.0 } };
         }
      }
   }

   template <typename Iterator>
   [[nodiscard]] auto superposition(Iterator first, Iterator last) noexcept
   {
      auto result = detail::null_force<typename std::iterator_traits<Iterator>::value_type>();
      return std::accumulate(first, last, result);
   }
}
