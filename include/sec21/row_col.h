#pragma once

#include <algorithm>
#include <type_traits>
#include <vector>

namespace sec21
{
   enum class row {};
   enum class col {};

   template <typename T>
   struct is_row : std::is_same<T, row>
   {
   };

   template <typename T>
   struct is_col : std::is_same<T, col>
   {
   };

   row& operator++(row& r) noexcept
   {
      using ut = typename std::underlying_type<row>::type;
      r = static_cast<row>(static_cast<ut>(r) + 1);
      return r;
   }

   template <typename Iterator>
   requires std::is_same_v<typename std::iterator_traits<Iterator>::value_type, row>
   auto row_to_col(Iterator first, Iterator last) noexcept
   {
      std::vector<col> result;
      std::transform(first, last, std::back_inserter(result), [](auto&& r) { return col{static_cast<int>(r)}; });
      return result;
   }
} // namespace sec21
