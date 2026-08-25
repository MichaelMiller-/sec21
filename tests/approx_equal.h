#pragma once

#include <catch.hpp>

#include <algorithm>

class approximately_equal
{
   double deviation{};

 public:
   //! \param epsilon 0.1 ... 0.0000000001
   explicit approximately_equal(double epsilon = 0.001) : deviation{epsilon} {}

   template <typename T>
   bool operator()(T const& lhs, T const& rhs)
   {
      return Catch::Approx(lhs).epsilon(deviation) == rhs;
   }
};

#if 0
template <typename Iterator1, typename Iterator2, typename Compare>
bool approx_equal(Iterator1 first1, Iterator1 last1, Iterator2 first2, Compare&& compare) noexcept(
   noexcept(std::is_nothrow_invocable_v<Compare, typename std::iterator_traits<Iterator1>::value_type,
                                        typename std::iterator_traits<Iterator2>::value_type>))
{
   return std::equal(first1, last1, first2, compare);
}
//! \todo implement high-order function
template <typename Rng1, typename Rng2, typename Compare>
bool approx_equal(Rng1&& rng1, Rng2&& rng2, Compare&& compare)
{
   return approx_equal(begin(rng1), end(rng1), begin(rng2), std::forward<Compare>(compare));
}
#endif

template <typename Iterator1, typename Iterator2>
bool approx_equal(Iterator1 first1, Iterator1 last1, Iterator2 first2, double deviation) noexcept
{
   return std::equal(first1, last1, first2, [deviation](auto const& lhs, auto const& rhs) {
      return Catch::Approx(lhs).epsilon(deviation) == rhs;
   });
}

template <typename Rng1, typename Rng2>
bool approx_equal(Rng1&& rng1, Rng2&& rng2, double deviation)
{
   return std::equal(begin(rng1), end(rng1), begin(rng2), approximately_equal{deviation});
}
