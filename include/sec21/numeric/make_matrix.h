#pragma once

#include <boost/numeric/ublas/matrix.hpp>

namespace sec21::numeric
{
   template <typename Allocator, typename Iterator, typename F = boost::numeric::ublas::row_major>
   [[nodiscard]] auto make_matrix(std::size_t m, std::size_t n, Iterator first, Iterator last)
   {
      if(m * n != std::distance(first, last)) {
         throw std::invalid_argument("expected matrix size didn't match with sequence length");
      }
      using value_t = typename std::iterator_traits<Iterator>::value_type;
      boost::numeric::ublas::unbounded_array<value_t, Allocator> storage(m*n, Allocator{});
      std::copy(first, last, storage.begin());
      return boost::numeric::ublas::matrix<value_t, F, decltype(storage)>(m, n, storage);
   }

   template <typename T, typename Allocator, typename F = boost::numeric::ublas::row_major>
   [[nodiscard]] auto make_matrix(std::size_t m, std::size_t n, T initial_value)
   {
      boost::numeric::ublas::unbounded_array<T, Allocator> storage(m * n, Allocator{});
      std::fill_n(storage.begin(), m * n, initial_value);
      return boost::numeric::ublas::matrix<T, F, decltype(storage)>(m, n, storage);
   }   
} // namespace sec21::numeric

