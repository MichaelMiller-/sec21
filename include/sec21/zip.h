#pragma once

#include <iterator>
#include <type_traits>

namespace sec21
{
   template <typename Callable, typename Iterator, typename... Iterators>
   void zip(Callable operation, Iterator first, Iterator last, Iterators... iterators) noexcept(
      std::is_nothrow_invocable_v<Callable, typename std::iterator_traits<Iterator>::value_type,
                                  typename std::iterator_traits<Iterators>::value_type...>)
   {
      for (; first != last; ++first, (++iterators, ...)) {
         operation(*first, *(iterators)...);
      }
   }
} // namespace sec21