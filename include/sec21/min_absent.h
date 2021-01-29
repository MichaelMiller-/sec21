#pragma once

#include <algorithm>

namespace sec21
{
#ifdef __cpp_concepts   
   template <std::forward_iterator Iterator, typename T = std::iter_value_t<Iterator>>
#else
   template <typename Iterator, typename T = typename std::iterator_traits<Iterator>::value_type>
#endif   
   constexpr auto min_absent(Iterator first, Iterator last, T initial = {}) noexcept
   {
      using difference_t = decltype(initial - initial);
      while (last != first)
      {
         auto const half = (std::distance(first, last) + 1) / 2;
         T const m = initial + static_cast<difference_t>(half);
         auto const pos = std::partition(first, last, [&](auto const &x) { return x < m; });
         if (pos == std::next(first, half))
         {
            first = pos;
            initial = m;
         }
         else
            last = pos;
      }
      return initial;
   }
} // namespace sec21
