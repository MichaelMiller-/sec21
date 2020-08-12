#pragma once

#include <boost/numeric/ublas/matrix.hpp>

#include <algorithm>

namespace sec21::numeric
{
   template <typename T, typename Allocator>
   [[nodiscard]] auto
   flatten(boost::numeric::ublas::matrix<T, boost::numeric::ublas::row_major, Allocator> input) noexcept
      -> std::vector<T>
   {
      std::vector<T> result(input.size1() * input.size2(), {});
      const auto data = input.data();
      std::copy(begin(data), end(data), begin(result));
      return result;
   }
} // namespace sec21::numeric
