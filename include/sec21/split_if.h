#pragma once

#include <iterator>
#include <type_traits>

namespace sec21
{
   template <typename InputIterator, typename OutputIterator, typename UnaryPredicate>
   void
   split_if(InputIterator first, InputIterator last, OutputIterator d_first1, OutputIterator d_first2,
            UnaryPredicate predicate) noexcept(noexcept(std::is_nothrow_invocable_v<UnaryPredicate,
                                                                                    typename std::iterator_traits<
                                                                                       InputIterator>::value_type>))
   {
      for (; first != last; ++first) {
         if (predicate(*first)) {
            *d_first1 = *first;
            ++d_first1;
         } else {
            *d_first2 = *first;
            ++d_first2;
         }
      }
   }
} // namespace sec21