#pragma once

#include <sstream>
#include <string>

namespace sec21
{
   //! \brief join string-convertible objects into one string
   // eg: std::string s = concat<char>("foo ", 42, " bar ", streamable_object, "value2");
   template <typename CharT, typename... Args>
   [[nodiscard]] auto concat(Args&&... args) noexcept
   {
      std::basic_stringstream<CharT> stream;
      (stream << ... << std::forward<Args>(args));
      return stream.str();
   }

   template <typename... Ts, typename UnaryOperation>
   [[nodiscard]] auto concat(std::tuple<Ts...> const& tuple, std::string_view delimiter, UnaryOperation unary_op)
   {
      std::stringstream stream{};
      std::string_view separator = "";
      std::size_t index{0};

      const auto worker = [&](auto const& value) {
         if (index++ > 0) {
            stream << delimiter;
         }
         stream << unary_op(value);
      };

      std::apply([&](auto const&... value) { (worker(value), ...); }, tuple);

      return stream.str();
   }

   template <typename... Ts>
   [[nodiscard]] auto concat(std::tuple<Ts...> const& tuple, std::string_view delimiter = ", ")
   {
      return concat(tuple, delimiter, [](auto const& value) noexcept { return value; });
   }

} // namespace sec21