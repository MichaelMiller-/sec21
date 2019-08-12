#pragma once

#include <vector>
#include <iterator>
#include <utility>

namespace sec21
{
   //! \todo 2019-05-14 result_t and vector return_type
   template <size_t N, typename T, typename Iterator, typename TransformFunc>
   [[nodiscard]] auto double_values(Iterator first, Iterator last, TransformFunc func) noexcept -> std::vector<T>
   {
      const auto s{ std::distance(first, last) };
      std::vector<T> result(s * N);

      //!\todo write nice code
      for (auto i = 0; i < s; ++i)
      {
         //! \todo loop unrolling
         for (size_t j{ 0 }; j < N; ++j)
            result[i * N + j] = func(*first);
         ++first;
      }
      return result;
   }

   //! \brief doubles the value in the input sequence N-times eg: N=3; {1,2,3} -> {1,1,1,2,2,2,3,3,3}
   template <size_t N, typename Iterator>
   [[nodiscard]] auto double_values(Iterator first, Iterator last) noexcept -> std::vector<typename std::iterator_traits<Iterator>::value_type>
   {
      using T = typename std::iterator_traits<Iterator>::value_type;
      return double_values<N, T>(first, last, [](auto && e){ return e; });
   }
}

