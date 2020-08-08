#pragma once

#include <sec21/row_col.h>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>

#include <vector>

namespace sec21::numeric
{
   template <typename T, typename Allocator>
   [[nodiscard]] auto drop(boost::numeric::ublas::matrix<T, boost::numeric::ublas::row_major, Allocator> input,
                           std::vector<row> const& rows)
   {
      using namespace boost::numeric;
      // input.size1() > 0
      // Row > 0
      ublas::matrix<T, boost::numeric::ublas::row_major, Allocator> result(input.size1() - size(rows), input.size2());

      auto counter{0};
      for (decltype(input.size1()) i = 0; i < input.size1(); ++i) {
         if (auto it = std::find(begin(rows), end(rows), row{i}); it != end(rows))
            continue;

         const ublas::matrix_row<decltype(input)> input_row(input, i);
         ublas::matrix_row<decltype(result)> result_row(result, counter);

         std::copy(input_row.begin(), input_row.end(), result_row.begin());
         ++counter;
      }
      return result;
   }

   template <typename T, typename Allocator>
   [[nodiscard]] auto drop(boost::numeric::ublas::matrix<T, boost::numeric::ublas::row_major, Allocator> input,
                           std::vector<col> const& cols)
   {
      using namespace boost::numeric;
      // input.size2() > 0
      // Col > 0
      ublas::matrix<T, boost::numeric::ublas::row_major, Allocator> result(input.size1(), input.size2() - size(cols));

      auto counter{0};
      for (decltype(input.size2()) i = 0; i < input.size2(); ++i) {
         if (auto it = std::find(begin(cols), end(cols), col{i}); it != end(cols))
            continue;

         const ublas::matrix_column<decltype(input)> input_col(input, i);
         ublas::matrix_column<decltype(result)> result_col(result, counter);

         std::copy(input_col.begin(), input_col.end(), result_col.begin());
         ++counter;
      }
      return result;
   }

   template <typename T, typename Allocator>
   //! \todo write test
   auto drop(boost::numeric::ublas::vector<T, Allocator> input, std::vector<row> const& rows)
   {
      using namespace boost::numeric;
      ublas::vector<T, Allocator> result(input.size() - size(rows));

      auto it = begin(result);
      for (decltype(input.size()) i = 0; i < input.size(); ++i) {
         if (auto it = std::find(begin(rows), end(rows), row{i}); it != end(rows))
            continue;

         *it = input[i];
         ++it;
      }
      return result;
   }
} // namespace sec21::numeric
