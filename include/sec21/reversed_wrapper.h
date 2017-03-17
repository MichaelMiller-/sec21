#pragma once

template <typename T>
struct reversed_wrapper 
{
   T const& v_;
 
   reversed_wrapper(T const& v) : v_{v} {}

   auto begin() { return std::rbegin(v_); }

   auto end() { return std::rend(v_); }
};
 
template <typename T>
reversed_wrapper<T> reversed(T const& v) {
   return reversed_wrapper<T>(v);
}

// or boost::adaptors::reverse(arr)