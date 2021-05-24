#pragma once

#include <catch.hpp>

#include <algorithm>

#if defined(CATCH_VERSION_MAJOR) // && defined(CATCH_VERSION_MINOR)

   class approximately_equal
   {
      double deviation{};
   public:
      //! \param epsilon 0.1 ... 0.0000000001
      explicit approximately_equal(double epsilon = 0.001) 
         : deviation{ epsilon } 
      {}

      template <typename T>
      bool operator()(T const& lhs, T const& rhs)
      {
         return Approx(lhs).epsilon(deviation) == rhs;
      }
   };

#endif


class logically_equal
{
   double deviation{};
public:
   //! \param epsilon 0.1 ... 0.0000000001
   explicit logically_equal(double epsilon = 0.001) 
      : deviation{ epsilon } 
   {}

   template <typename T>
   bool operator()(T const& lhs, T const& rhs) noexcept
   {
      return std::fabs(lhs - rhs) < deviation;
   }
};

template <typename Iterator1, typename Iterator2, typename Compare>
bool approx_equal(Iterator1 first1, Iterator1 last1, Iterator2 first2, Compare&& compare) 
//! \todo noexcept(std::invoke(compare, Iterator1::value_type{}))
{
   return std::equal(first1, last1, first2, compare); 
}

template <typename Iterator1, typename Iterator2>
bool approx_equal(Iterator1 first1, Iterator1 last1, Iterator2 first2, double deviation) noexcept
{
   return std::equal(
      first1, 
      last1, 
      first2, 
      [deviation](auto const& lhs, auto const& rhs) { return Approx(lhs).epsilon(deviation) == rhs; }); 
}

//! \todo implement high-order function 
template <typename Rng1, typename Rng2, typename Compare>
bool approx_equal(Rng1 const& rng1, Rng2 const& rng2, Compare&& compare) noexcept
{
   return approx_equal(std::begin(rng1), std::end(rng1), std::begin(rng2), std::forward<Compare>(compare));
}

template <typename Rng1, typename Rng2>
bool approx_equal(Rng1 const& rng1, Rng2 const& rng2, double deviation) noexcept
{
   return std::equal(std::begin(rng1), std::end(rng1), std::begin(rng2), approximately_equal{ deviation });
}
