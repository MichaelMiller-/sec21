#include "catch.hpp"

#include <algorithm>

// Vergleich mit einer n-Prozent Abweichung
template <typename Iterator1, typename Iterator2>
bool approx_equal(Iterator1 first1, Iterator1 last1, Iterator2 first2, double divergence) noexcept
{
   return std::equal(
      first1, 
      last1, 
      first2, 
      [divergence](auto const& lhs, auto const& rhs) { return Approx(lhs).epsilon(divergence) == rhs; }); 
}

// Vergleich mit einer n-Prozent Abweichung
template <typename Rng1, typename Rng2>
bool approx_equal(Rng1 const& rng1, Rng2 const& rng2, double divergence) noexcept
{
   return approx_equal(std::begin(rng1), std::end(rng1), std::begin(rng2), divergence);
}
