#pragma once

#include <iterator>
#include <functional>

namespace sec21
{
   template <typename Sequence, typename Predicate>
   auto erase_if_index(Sequence&& seq, Predicate pred)
   {
      auto result{seq};
      auto first = std::begin(result);
      auto i{0};

      for (auto it = std::begin(seq); it != std::end(seq); ++it, ++i)
      {
         if (std::invoke(pred, i))
            first = result.erase(first);
         else
            ++first;
      }
      return result;
   }
}
