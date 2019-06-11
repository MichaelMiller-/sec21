#pragma once

#include <iterator>
#include <tuple>

namespace sec21 
{
   template <
      typename T,
      typename Iterator = decltype(std::begin(std::declval<T>())),
      typename = decltype(std::end(std::declval<T>()))>
   constexpr auto enumerate(T && iterable) noexcept
   {
      struct iterator
      {
         size_t i{0};
         Iterator iter;

         bool operator != (iterator const& other) const noexcept { return iter != other.iter; }

         void operator ++ () noexcept { ++i; ++iter; }
         auto operator * () const noexcept { return std::tie(i, *iter); }
      };

      struct iterable_wrapper
      {
         T iterable;

         auto begin() noexcept { return iterator{ 0, std::begin(iterable) }; }
         auto end() noexcept { return iterator{ 0, std::end(iterable) }; }
      };

      return iterable_wrapper{ std::forward<T>(iterable) };
   }
}
