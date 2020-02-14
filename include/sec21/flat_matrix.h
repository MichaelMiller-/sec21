#pragma once

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>

#include <sec21/flat_vector.h>

namespace sec21
{
   template <typename T, typename F = boost::numeric::ublas::row_major>
   [[nodiscard]] auto flat_matrix(boost::numeric::ublas::matrix<T, F> in) noexcept 
   // -> decltype(flat_vector<T, Iterator>)
   {
      namespace bnu = boost::numeric::ublas;
      std::vector<std::vector<T>> result;

      for (decltype(in.size1()) i = 0; i < in.size1(); ++i)
      {
         std::vector<T> row(in.size2());
         bnu::matrix_row<bnu::matrix<T, F>> mr(in, i); 

         std::copy(mr.begin(), mr.end(), row.begin());
         result.push_back(row);
      }
      return flat_vector<T>(std::begin(result), std::end(result));
   }
}
