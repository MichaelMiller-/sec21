#pragma once

#include <ostream>
#include <string_view>
#include <iterator>
#include <algorithm>
#include <sstream>
#include <tuple>

namespace sec21
{
   template <typename Sequence>
   [[deprecated("replace with fmt::join")]][[maybe_unused]] auto& enclose(
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
   [[deprecated("replace with fmt::join")]][[nodiscard]] auto enclose(
      Sequence&& seq,
      std::string_view delimiter = ", ",
      std::string_view start = "{",
      std::string_view end = "}") -> std::string
   {
      std::stringstream ss{};
      enclose(ss, std::forward<Sequence>(seq), delimiter, start, end);
      return ss.str();
   }

   namespace detail
   {
      template <typename... Args, typename Function, auto... Idx>
      void for_each(std::tuple<Args...> const& t, Function&& func, std::index_sequence<Idx...>)
      {
         (func(std::get<Idx>(t)), ...);
      }
   }

   template <typename... Args, typename TransformFunction>
   [[deprecated("replace with fmt::join")]] auto enclose(
      std::tuple<Args...> args,
      TransformFunction&& func,
      std::string_view delimiter = ",",
      std::string_view start = "(",
      std::string_view end = ")")
   {
      constexpr auto N = std::tuple_size_v<decltype(args)>;
      static_assert(N > 0, "cannot enclose a empty tuple");

      std::stringstream ss{};
      ss << start;
      detail::for_each(args, [&](auto&& v) { ss << func(v) << delimiter; }, std::make_index_sequence<N-1>{});
      ss << func(std::get<N-1>(args));
      ss << end;
      return ss.str();
   }

   template <typename... Args>
   [[deprecated("replace with fmt::join")]] auto enclose(
      std::tuple<Args...> args,
      std::string_view delimiter = ",",
      std::string_view start = "(",
      std::string_view end = ")")
   {
      return enclose(args, [](auto v){ return v; }, delimiter, start, end);
   }
} // sec21
