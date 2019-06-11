#pragma once

namespace sec21
{
   //! \brief visitor pattern like "overload"
   //! \brief https://en.cppreference.com/w/cpp/utility/variant/visit
   //
   template <typename... Ts> struct visitor_t : Ts... { using Ts::operator()...; };
   template <typename... Ts> visitor_t(Ts...) -> visitor_t<Ts...>;
}
