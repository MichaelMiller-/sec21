#pragma once

#include <iterator>
#include <tuple>

namespace sec21 
{
   template <
      typename T,
      typename Iterator = decltype(std::begin(std::declval<T>())),
      typename = decltype(std::end(std::declval<T>()))>
   constexpr auto enumerate(T && iterable)
   {
      struct iterator
      {
         size_t i{0};
         Iterator iter;

         bool operator != (iterator const& other) const { return iter != other.iter; }

         void operator ++ () { ++i; ++iter; }
         auto operator * () const { return std::tie(i, *iter); }
      };
      struct iterable_wrapper
      {
         T iterable;

         auto begin() { return iterator{ 0, std::begin(iterable) }; }
         auto end() { return iterator{ 0, std::end(iterable) }; }
      };

      return iterable_wrapper{ std::forward<T>(iterable) };
   }
}
