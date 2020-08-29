#pragma once

#include <vector>
#include <algorithm>
#include <type_traits>
#include <exception>
#include <limits>

//! \todo remove

namespace sec21
{

template <typename T>
struct invalid_key {};

template <>
struct invalid_key<int> : std::integral_constant<int, std::numeric_limits<int>::max()> {};

template <>
struct invalid_key<size_t> : std::integral_constant<size_t, std::numeric_limits<size_t>::max()> {};

template <typename Key1, typename Key2, typename Value, auto Dim = 2>
//! \todo Key2 should have some kind of null_handle
//! \todo requirement Key2 should be convertiable to int
class lookup
{
   std::vector<Value> values;
   std::vector<Key1> keys;

   struct proxy
   {
      std::vector<Value> &values;
      int start_index{0};

      auto operator[](Key2 &&key) const
      {
         // if (invalid_key<Key2>::value == key)
         //    throw std::invalid_argument("invalid key: {}");
         return values[start_index * Dim + static_cast<int>(key)];
      }

      auto &operator[](Key2 &&key)
      {
         // if (invalid_key<Key2>::value == key)
         //    throw std::invalid_argument("invalid key: {}");
         return values[start_index * Dim + static_cast<int>(key)];
      }
   };

 public:
   using key1_t = Key1;
   using key2_t = Key2;
   using value_t = Value;

   void clear()
   {
      values.clear();
      keys.clear();
   }

   //! \todo name
   //! \todo clear
   template <typename Iterator>
// #ifdef __cpp_concepts
//       requires std::is_same_v<std::iterator_traits<Iterator>::value_type, key1_t>
// #endif
   auto reserve(Iterator first, Iterator last)
   {
      // static_assert(std::is_same_v<typename std::iterator_traits<Iterator>::value_type, Key1>, "iterator::value_type must be the same type as Key1");

      values.resize(std::distance(first, last) * Dim);
      // std::fill(std::begin(values), std::end(values), invalid_key<key1_t>::value);
      std::copy(first, last, std::back_inserter(keys));

      // values.erase(std::remove_if(std::begin(values), std::end(values),[](auto&& e){}), std::end(values));
   }

   auto operator[](Key1 const& key)
   {
      if (auto it = std::find(std::begin(keys), std::end(keys), key); it != std::end(keys))
         return proxy{values, std::distance(std::begin(keys), it)};
      throw std::invalid_argument("invalid key");
   }

   auto raw_values_begin() const { return values.begin(); }
   auto raw_values_end() const { return std::end(values); }

   constexpr auto size() const noexcept { return std::size(values); }
};
} // namespace sec21
