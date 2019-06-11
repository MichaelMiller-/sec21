#pragma once

#include <vector>
#include <algorithm>

namespace sec21
{
   namespace detail
   {
      template <typename Input, typename Output>
      struct flat_vector_impl
      {
         template <typename InputIterator, typename OutputIterator>
         static void apply(InputIterator first, InputIterator last, OutputIterator out) noexcept
         {
            for (; first != last; ++first) 
            {
               using iter_t = typename Input::iterator;
               using value_t = typename std::iterator_traits<iter_t>::value_type;

               detail::flat_vector_impl<value_t, Output>::apply(std::begin(*first), std::end(*first), out);
            }
         }
      };

      template <typename Input>
      struct flat_vector_impl<Input, Input>
      {
         template <typename InputIterator, typename OutputIterator>
         static void apply(InputIterator first, InputIterator last, OutputIterator out) noexcept
         {
            std::copy(first, last, out);
         }
      };
   }

   template <typename T, typename InputIterator>
   [[nodiscard]] auto flat_vector(InputIterator first, InputIterator last) noexcept -> std::vector<T>
   {
      using value_t = typename std::iterator_traits<InputIterator>::value_type;
      std::vector<T> result;
      detail::flat_vector_impl<value_t, T>::apply(first, last, std::back_inserter(result));
      return result;
   }
}
