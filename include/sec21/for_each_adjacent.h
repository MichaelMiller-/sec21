#pragma once

#include <functional>

namespace sec21
{
   template <typename Iterator, class Callable>
   auto for_each_adjacent(Iterator first, Iterator last, Callable&& func) noexcept(
      std::is_nothrow_invocable_v<Callable, typename std::iterator_traits<Iterator>::value_type,
                                  typename std::iterator_traits<Iterator>::value_type>)
   {
      if (first == last) {
         return;
      }

      auto next = first;
      ++next;

      for (; next != last; ++next, ++first) {
         std::invoke(std::forward<Callable>(func), *first, *next);
      }
   }

   template <typename Sequence, class Callable>
   auto for_each_adjacent(Sequence&& sequence, Callable&& func) noexcept(
      std::is_nothrow_invocable_v<Callable, typename std::decay_t<Sequence>::value_type,
                                  typename std::decay_t<Sequence>::value_type>)
   {
      return for_each_adjacent(begin(sequence), end(sequence), std::forward<Callable>(func));
   }
} // namespace sec21
