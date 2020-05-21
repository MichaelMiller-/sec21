#pragma once

#include <ostream>
#include <string_view>
#include <iterator>
#include <algorithm>
#include <sstream>

namespace sec21
{
   template <typename Sequence>
   [[maybe_unused]] auto& enclose(
      std::ostream& os,
      Sequence const& seq,
      std::string_view delimiter = ", ",
      std::string_view start = "{",
      std::string_view end = "}")
   {
      using T = typename Sequence::value_type;

      os << start;
      if (std::empty(seq) == false) 
      {
         std::copy(std::begin(seq), std::prev(std::end(seq)), std::ostream_iterator<T>(os, delimiter.data()));
         os << *std::rbegin(seq);
      }
      os << end;
      return os;
   }

   template <typename Sequence>
   [[nodiscard]] auto enclose(
      Sequence&& seq,
      std::string_view delimiter = ", ",
      std::string_view start = "{",
      std::string_view end = "}") -> std::string
   {
      std::stringstream ss{};
      enclose(ss, std::forward<Sequence>(seq), delimiter, start, end);
      return ss.str();
   }
} // sec21
