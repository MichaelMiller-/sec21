#pragma once

namespace sec21
{
   namespace detail
   {
      template <auto ChunkSize>
      class for_each_chunk_impl
      {
         static_assert(ChunkSize > 1);

         template <typename Iterator, typename Callable, std::size_t ...Is>
         constexpr auto invoke(Iterator first, Callable func, std::index_sequence<Is...>) const
         {
            return std::invoke(func, *std::next(first, Is)...);
         }

      public:
         template <typename Iterator, typename Callable>
         constexpr auto operator()(Iterator first, Iterator last, Callable&& func) const
         {
            while (std::distance(first, last) >= ChunkSize)
            {
                  invoke(first, std::forward<Callable>(func), std::make_index_sequence<ChunkSize>{});
                  std::advance(first, ChunkSize);
            }
         }
      };
   }

   template <auto ChunkSize, typename Iterator, typename Callable>
   void for_each_chunk(Iterator first, Iterator last, Callable&& func)
   {
      return detail::for_each_chunk_impl<ChunkSize>()(first, last, std::forward<Callable>(func));
   }
}
