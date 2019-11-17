#pragma once

#include <vector>
#include <algorithm>
#include <type_traits>
#include <exception>

namespace sec21
{
template <typename Key1, typename Key2, typename Value, auto Dim = 2>
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
         return values[start_index * Dim + static_cast<int>(key)];
      }

      auto &operator[](Key2 &&key)
      {
         return values[start_index * Dim + static_cast<int>(key)];
      }
   };

 public:
   using key1_t = Key1;
   using key2_t = Key2;

   void clear()
   {
      values.clear();
      keys.clear();
   }

   //! \todo name
   //! \todo clear
   template <typename Iterator>
#if __cpp_conditional_explicit
   explicit(std::is_same_v<std::iterator_traits<Iterator>::value_type, Key>)
#endif
       auto reserve(Iterator first, Iterator last)
   {
      static_assert(std::is_same_v<typename std::iterator_traits<Iterator>::value_type, key_t>, "iterator::value_type must be the same type as Key1");

      values.resize(std::distance(first, last) * Dim);
      std::copy(first, last, std::back_inserter(keys));
   }

   auto operator[](Key1 key)
   {
      if (auto it = std::find(std::begin(keys), std::end(keys), key); it != std::end(keys))
         return proxy{values, std::distance(std::begin(keys), it)};
      throw std::invalid_argument("wtf");
   }

   auto raw_values_begin() const { return values.begin(); }
   auto raw_values_end() const { return std::end(values); }
};
} // namespace sec21
