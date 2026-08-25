#pragma once

#include <iterator>
#include <type_traits>

namespace sec21
{
   template <typename InputIterator, typename OutputIterator, typename UnaryPredicate, typename UnaryOperation>
#if __cpp_concepts >= 201907L
      requires requires(InputIterator, OutputIterator, UnaryOperation)
   {
      requires std::is_convertible_v<std::invoke_result_t<UnaryPredicate, typename std::iterator_traits<InputIterator>::value_type>, bool>;
      requires std::is_convertible_v<std::invoke_result_t<UnaryOperation, typename std::iterator_traits<InputIterator>::value_type>, typename OutputIterator::container_type::value_type>;
   }
#endif
   OutputIterator transform_if(
      InputIterator first,
      InputIterator last,
      OutputIterator d_first,
      UnaryPredicate predicate,
      UnaryOperation unary_op)
   {
      for (; first != last; ++first)
      {
         if (predicate(*first))
         {
            *d_first = unary_op(*first);
            ++d_first;
         }
      }
      return d_first;
   }
} // namespace sec21