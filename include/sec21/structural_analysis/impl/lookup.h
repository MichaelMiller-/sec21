#pragma once

#include <sec21/structural_analysis/impl/axis.h>

#include <vector>
#include <algorithm>
#include <type_traits>
#include <functional>
#include <numeric>

namespace sec21::structural_analysis::impl
{
   template <typename System, typename T>
   auto make_lookup(System const& sys, T initial_value) noexcept -> std::vector<T>
   {
      const auto n = std::distance(std::begin(sys.nodes), std::end(sys.nodes));
      if (n == 0)
         return {};

      std::vector<T> result(n * System::dimension_v);
      std::iota(std::begin(result), std::end(result), std::move(initial_value));
      return result;
   }

   template <
      typename InputIterator1, 
      typename InputIterator2, 
      typename OutputIterator1,
      typename OutputIterator2>
   //! \todo possible to implement as a real partition and return the pivot iterator 
   auto partition_lookup(
      InputIterator1 first1, 
      InputIterator1 last1, 
      InputIterator2 first2, 
      InputIterator2 last2, 
      OutputIterator1 out_true,
      OutputIterator2 out_false) -> std::pair<OutputIterator1, OutputIterator2>
   {
      static_assert(std::is_convertible_v<typename std::iterator_traits<InputIterator2>::value_type, bool>);

      while (first1 != last1 && first2 != last2) 
      {
         if (*first2 == true)
            *out_true++ = *first1;
         else
            *out_false++ = *first1;
         ++first1;
         ++first2;
      }
      return { out_true, out_false };
   }

} // namespace sec21::structural_analysis
