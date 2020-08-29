#pragma once

#include <variant>

//! \todo delete
namespace sec21
{
   //! \brief visitor pattern like "overload"
   //! \brief https://en.cppreference.com/w/cpp/utility/variant/visit
   //
   template <typename... Ts> struct visitor_t : Ts... { using Ts::operator()...; };
   template <typename... Ts> visitor_t(Ts...) -> visitor_t<Ts...>;

   template <typename Variant, typename... Matchers>
   auto match(Variant&& variant, Matchers&&... matchers)
   {
      return std::visit(visitor_t{std::forward<Matchers>(matchers)...}, std::forward<Variant>(variant));
   }
}
