#pragma once

#include <functional>

namespace sec21
{
   namespace detail
   {
      template <auto ChunkSize>
      class for_each_chunk_impl
      {
         static_assert(ChunkSize > 1);

         template <typename Callable, typename... Args>
         constexpr void invoke(Callable&& func, Args&&... args) const
            noexcept(std::is_nothrow_invocable_v<Callable, Args...>)
         {
            std::invoke(std::forward<Callable>(func), std::forward<Args>(args)...);
         }

         template <typename Iterator, typename Callable, auto... Is>
         constexpr void invoke(Iterator first, Callable&& func, std::index_sequence<Is...>) const
         {
            invoke(std::forward<Callable>(func), *std::next(first, Is)...);
         }

       public:
         template <typename Iterator, typename Callable>
         constexpr auto operator()(Iterator first, Iterator last, Callable&& func) const
         {
            if (auto result = std::distance(first, last) % ChunkSize; result != 0) {
               throw std::runtime_error("Number of elements cannot be divided by ChunkSize");
            }

            while (std::distance(first, last) >= ChunkSize) {
               invoke(first, std::forward<Callable>(func), std::make_index_sequence<ChunkSize>{});
               std::advance(first, ChunkSize);
            }
         }
      };
   } // namespace detail

   template <auto ChunkSize, typename Iterator, typename Callable>
   void for_each_chunk(Iterator first, Iterator last, Callable&& func)
   {
      detail::for_each_chunk_impl<ChunkSize>()(first, last, std::forward<Callable>(func));
   }

   template <auto ChunkSize, typename Range, typename Callable>
   void for_each_chunk(Range&& range, Callable&& func)
   {
      for_each_chunk<ChunkSize>(begin(range), end(range), std::forward<Callable>(func));
   }
} // namespace sec21
