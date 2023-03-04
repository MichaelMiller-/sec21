#pragma once

#include <sec21/reflection/table.h>

#include <tuple>

namespace sec21::reflection
{
   template <auto Index, typename T>
   constexpr decltype(auto) get_column(T&& t) noexcept
   {
      using reflection_t = typename table<std::decay_t<T>>::metainfo;
      static_assert(Index < std::tuple_size_v<reflection_t>, "Index is out of range");
      return std::get<Index>(reflection_t{}).get(t);
   }
} // namespace sec21::reflection