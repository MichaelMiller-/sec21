#pragma once
#pragma once

#include <vector>
#include <numeric>
#include <cmath>
#include <iterator>

namespace sec21
{
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
