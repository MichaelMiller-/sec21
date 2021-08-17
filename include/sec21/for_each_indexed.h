#pragma once

#include <functional>
#include <iterator>

namespace sec21
{
   template <typename Iterator, typename Callable>
   auto for_each_indexed(
      Iterator first, Iterator last, typename std::iterator_traits<Iterator>::difference_type initial,
      Callable
         func) noexcept(noexcept(std::is_nothrow_invocable_v<Callable,
                                                             typename std::iterator_traits<Iterator>::difference_type,
                                                             typename std::iterator_traits<Iterator>::value_type>))
   {
      for (; first != last; ++first, ++initial)
         std::invoke(func, initial, *first);
      return func;
   }

   template <typename Iterator, typename Callable>
   auto for_each_indexed(Iterator first, Iterator last, Callable func) //! \todo noexcept?
   {
      return for_each_indexed(first, last, 0, func);
   }

   template <typename Sequence, typename Callable>
   auto for_each_indexed(Sequence const& seq, Callable&& func)  //! \todo noexcept?
   {
      return for_each_indexed(std::begin(seq), std::end(seq), 0, std::forward<Callable>(func));
   }
} // namespace sec21
