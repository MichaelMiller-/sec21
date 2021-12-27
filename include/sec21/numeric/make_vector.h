#pragma once

#include <boost/numeric/ublas/vector.hpp>

#include <algorithm>
#include <memory>

namespace sec21::numeric
{
   template <typename Allocator, typename Iterator>
   auto make_vector(Iterator first, Iterator last)
   {
      using value_t = typename std::iterator_traits<Iterator>::value_type;
      const auto n = std:: distance(first, last);
      boost::numeric::ublas::unbounded_array<value_t, Allocator> storage(n, Allocator{});
      std::copy(first, last, storage.begin());
      return boost::numeric::ublas::vector<value_t, decltype(storage)>(n, storage);
   }

   template <typename Allocator, typename Sequence>
   auto make_vector(Sequence&& sequence)
   {
      return make_vector<Allocator>(begin(sequence), end(sequence));
   }

   template <typename Allocator, typename Iterator, typename TransformFunc>
   auto make_vector(Iterator first, Iterator last, TransformFunc&& func)
   {
      using arg_t = typename std::iterator_traits<Iterator>::value_type;
      using value_t = std::invoke_result_t<TransformFunc, arg_t>;

      const auto n = std::distance(first, last);
      boost::numeric::ublas::unbounded_array<value_t, Allocator> storage(n, Allocator{});
      std::transform(first, last, storage.begin(), std::forward<TransformFunc>(func));
      return boost::numeric::ublas::vector<value_t, decltype(storage)>(n, storage);
   }
} // namespace sec21::numeric
