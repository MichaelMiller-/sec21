#pragma once

#include <variant>

namespace sec21
{
   //! \brief https://en.cppreference.com/w/cpp/utility/variant/visit
   template <typename... Ts>
   struct overloaded : Ts... 
   { 
      using Ts::operator()...; 
   };
   // explicit deduction guide
   template <typename... Ts> overloaded(Ts...) -> overloaded<Ts...>;

   template <typename Variant, typename... Matchers>
   auto match(Variant&& variant, Matchers&&... matchers)
   {
      return std::visit(overloaded{std::forward<Matchers>(matchers)...}, std::forward<Variant>(variant));
   }
}
