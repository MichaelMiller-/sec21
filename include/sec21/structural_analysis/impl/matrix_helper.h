#pragma once

#include <sec21/contains.h>
#include <sec21/structural_analysis/impl/lookup.h>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>

namespace sec21::structural_analysis::impl
{
   template <typename T = double, typename Iterator>
   // requires T == std::iterator_traits<Iterator>::value_type
   [[nodiscard]] auto make_vector(Iterator first, Iterator last) -> boost::numeric::ublas::vector<T>
   {
      using value_t = typename std::iterator_traits<Iterator>::value_type;
      const auto n = std::distance(first, last);
      boost::numeric::ublas::unbounded_array<T> storage(n);
      std::copy(first, last, storage.begin());
      return boost::numeric::ublas::vector<T>(n, storage);
   }     

   template <typename Iterator, typename F = boost::numeric::ublas::row_major>
   [[nodiscard]] auto make_matrix(std::size_t m, std::size_t n, Iterator first, Iterator last)
      -> boost::numeric::ublas::matrix<typename std::iterator_traits<Iterator>::value_type, F> 
   {
      if(m * n != std::distance(first, last)) {
         throw std::invalid_argument("expected matrix size didn't match with sequence length");
      }
      using value_t = typename std::iterator_traits<Iterator>::value_type;
      boost::numeric::ublas::unbounded_array<value_t> storage(m*n);
      std::copy(first, last, storage.begin());
      return boost::numeric::ublas::matrix<value_t>(m, n, storage);
   }
   
   template <typename T>
   auto remove_from_matrix(boost::numeric::ublas::matrix<T> in, std::vector<Row> const &rows, std::vector<Col> const &cols)
   {
      using namespace boost::numeric;
      ublas::matrix<T> result(in.size1() - std::size(rows), in.size2() - std::size(cols));

      auto rc{0};
      for (decltype(in.size1()) i = 0; i < in.size1(); ++i)
      {
         if (::sec21::contains(rows, Row{i}))
            continue;

         const ublas::matrix_row<ublas::matrix<T>> input_row(in, i);
         ublas::matrix_row<ublas::matrix<T>> result_row(result, rc);

         auto j{0};
         std::copy_if(input_row.begin(), input_row.end(), result_row.begin(), [&j, &cols](auto) mutable {
            bool result = (::sec21::contains(cols, Col{j}) == false);
            j++;
            return result;
         });
         ++rc;
      }
      return result;
   }   
} // namespace sec21::structural_analysis

