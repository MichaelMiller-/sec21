#pragma once

#include <tuple>

namespace sec21
{
   namespace detail
   {
      template <typename Iterator>
      void advance_all(Iterator &iterator)
      {
         ++iterator;
      }
      template <typename Iterator, typename... Iterators>
      void advance_all(Iterator &iterator, Iterators &... iterators)
      {
         ++iterator;
         advance_all(iterators...);
      }
   }

   template <typename Callable, typename Iterator, typename... Iterators>
   void zip(Callable func, Iterator begin, Iterator end, Iterators... iterators)
   {
      for (; begin != end; ++begin, detail::advance_all(iterators...))
      {
         func(std::make_tuple(*begin, *(iterators)...));
      }
   }   
}