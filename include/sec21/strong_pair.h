#pragma once

#include <utility>

namespace sec21
{
   //! \todo implement policies (like strong_type)
   template <typename First, typename Second>
   class strong_pair
   {
      First first;
      Second second;

   public:
      strong_pair() = delete;

      template <typename T, typename U>
      constexpr strong_pair(T && t, U && u)
         : first{ std::move(t) }
         , second{ std::move(u) }
      {} 

      constexpr bool operator == (strong_pair const& other) const noexcept {
         return first == other.first && second == other.second;
      }
   };

   template <typename T, typename U> strong_pair(T, U) -> strong_pair<T, U>;
}
