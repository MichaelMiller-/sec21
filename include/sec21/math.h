#pragma once

#include <numeric>
#include <cmath>
#include <iterator>

namespace sec21
{
#ifdef __cpp_fold_expressions
   
   //! \brief
   template <typename... Args>
   auto accumulate(Args ... args) noexcept
   {
      return (args + ...);
   }

   //! \brief
   template <typename... Args>
   auto average(Args &&... args) noexcept
   {
      return accumulate(std::forward<Args>(args)...) / sizeof...(Args);
   }

   //! \brief
   template <typename Arithmetic, typename... Args>
   decltype(auto) fmin(Arithmetic v, Args &&... args)
   {
      auto result{ v };
      ((result = std::fmin(result, args)), ...);
      return result;
   }
#endif

   //! \brief calculates the absolute norm of an array (Euklidische Norm)
   template <typename Iterator>
   constexpr auto norm(Iterator first, Iterator last) noexcept
   {
      return std::sqrt(
          std::accumulate(
              first, 
              last, 
              typename std::iterator_traits<Iterator>::value_type(), 
              [](auto const& a, auto const& b){ return a + (b * b); }));
   }
}
