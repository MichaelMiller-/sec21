#pragma once

namespace sec21
{
   //! \brief visitor pattern like "overload"
   //! \brief https://en.cppreference.com/w/cpp/utility/variant/visit
   //
   template <typename... Ts> struct visitor : Ts... { using Ts::operator()...; };
   template <typename... Ts> visitor(Ts...) -> visitor<Ts...>;
}
