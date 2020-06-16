#pragma once

#include <sec21/contains.h>
#include <sec21/structural_analysis/impl/lookup.h>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>

namespace sec21::structural_analysis::impl
{
   template <typename T>
   //! \todo write test
   auto remove_from_matrix(boost::numeric::ublas::matrix<T> in, std::vector<Row> const& rows, std::vector<Col> const& cols)
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

   template <typename T>
   //! \todo write test
   auto remove_from_vector(boost::numeric::ublas::vector<T> in, std::vector<Row> const& rows)
   {
      using namespace boost::numeric;
      ublas::vector<T> result(in.size() - std::size(rows));
      auto it = begin(result);

      for (decltype(in.size()) i = 0; i < in.size(); ++i)
      {
         if (::sec21::contains(rows, Row{i}))
            continue;

         *it = in[i];
         ++it;
      }      
      return result;
   }
} // namespace sec21::structural_analysis

