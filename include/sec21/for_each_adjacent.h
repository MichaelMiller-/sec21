#pragma once

#include <functional>

namespace sec21
{
   //! \todo write test
   template<typename Iterator, class Callable>
   auto for_each_adjacent(Iterator first, Iterator last, Callable&& func)
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

} // namespace sec21
