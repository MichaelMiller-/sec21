#pragma once

#include <cmath>
#include <iterator>
#include <numeric>

namespace sec21
{
#if __cpp_fold_expressions >= 201603L

   //! \brief
   template <typename... Args>
   [[nodiscard]] constexpr auto accumulate(Args... args) noexcept
   {
      return (args + ...);
   }

   //! \brief
   template <typename... Args>
   [[nodiscard]] constexpr auto average(Args&&... args) noexcept
   {
      return accumulate(std::forward<Args>(args)...) / sizeof...(Args);
   }

   //! \brief
#if __cpp_concepts >= 201907L
   template <std::floating_point Arithmetic, std::floating_point... Args>
#else
   template <typename Arithmetic, typename... Args>
#endif
   [[nodiscard]] constexpr auto fmin(Arithmetic v, Args&&... args)
   {
      auto result{v};
      ((result = std::fmin(result, args)), ...);
      return result;
   }
#endif

   //! \brief calculates the absolute norm of an array (Euklidische Norm)
   template <typename Iterator>
   [[nodiscard]] constexpr auto norm(Iterator first, Iterator last) noexcept
   {
      return std::sqrt(std::accumulate(first, last, typename std::iterator_traits<Iterator>::value_type(),
                                       [](auto const& a, auto const& b) { return a + (b * b); }));
   }
} // namespace sec21
