#pragma once

#include <sec21/structural_analysis/impl/axis.h>

#include <vector>
#include <algorithm>
#include <type_traits>
#include <functional>
#include <numeric>

namespace sec21::structural_analysis::impl
{
   enum class Row {};
   enum class Col {};

   template <typename T>
   struct is_row : std::is_same<T, Row> {};

   template <typename T>
   struct is_col : std::is_same<T, Col> {};

   // pre-increment behavior for Row
   Row& operator ++ (Row &r) noexcept 
   {
      using ut = typename std::underlying_type<Row>::type;
      r = static_cast<Row>(static_cast<ut>(r) + 1);
      return r;
   }

   template <typename System, typename T, typename Predicate>
   auto filter(System const& sys, T&& in, Predicate&& pred)
   {
      auto result{in};
      auto first = std::begin(result);

      for (auto it = std::begin(sys.nodes); it != std::end(sys.nodes); ++it)
      {
         for (auto i = 0; i < System::dimension_v; ++i)
         {
            if (std::invoke(pred, *it, Axis{i}) == false)
               first = result.erase(first);
            else
               ++first;
         }
      }
      return result;
   }

   template <typename System, typename T>
#ifdef __cpp_concepts
   requires is_row<T>::value || is_col<T>::value
#endif
   //! \todo default start with zero 
   auto make_lookup(System const& sys, T&& initial_value) noexcept -> std::vector<T>
   {
      const auto n = std::distance(std::begin(sys.nodes), std::end(sys.nodes));
      if (n == 0)
         return {};

      std::vector<T> result(n * System::dimension_v);
      std::iota(std::begin(result), std::end(result), std::forward<T>(initial_value));
      return result;
   }

   template <typename Iterator>
#ifdef __cpp_concepts
      requires std::is_same_v<typename std::iterator_traits<Iterator>::value_type, Row>
#endif
   auto row_to_col(Iterator first, Iterator last) noexcept
   {
      std::vector<Col> result;
      std::transform(first, last, std::back_inserter(result), [](auto &&r) {
         return Col{static_cast<int>(r)};
      });
      return result;
   }

   template <
      typename InputIterator1, 
      typename InputIterator2, 
      typename OutputIterator1,
      typename OutputIterator2>
   //! \todo possible to implement as a real partition and return the pivot iterator 
   auto partition_lookup(
      InputIterator1 first1, 
      InputIterator1 last1, 
      InputIterator2 first2, 
      InputIterator2 last2, 
      OutputIterator1 out_true,
      OutputIterator2 out_false) -> std::pair<OutputIterator1, OutputIterator2>
   {
      static_assert(std::is_convertible_v<typename std::iterator_traits<InputIterator2>::value_type, bool>);

      while (first1 != last1 && first2 != last2) 
      {
         if (*first2 == true)
            *out_true++ = *first1;
         else
            *out_false++ = *first1;
         ++first1;
         ++first2;
      }
      return { out_true, out_false };
   }

} // namespace sec21::structural_analysis
