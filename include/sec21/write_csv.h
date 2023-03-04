#pragma once

#include <sec21/concat.h>
#include <sec21/reflection/table.h>

#include <tuple>
#include <ostream>

namespace sec21
{
   template <typename Iterator>
   auto write_csv(std::ostream& os, Iterator first, Iterator last)
   {
      using value_t = typename std::iterator_traits<Iterator>::value_type;
      using reflection_t = typename reflection::table<value_t>::metainfo;

      os << concat(reflection_t{}, ", ", [](auto const& v) noexcept { return v.name; }) << std::endl;

      for (; first != last; ++first)
      {
         os << concat(std::apply([row = *first](auto const&... value) { return std::make_tuple(value.get(row)...); }, reflection_t{}), ", ") << std::endl;
      }
   }

   template <typename Container>
   auto write_csv(std::ostream& os, Container const& container)
   {
      write_csv(os, begin(container), end(container));
   }
}
