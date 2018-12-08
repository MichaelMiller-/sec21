#pragma once

#include <sec21/crtp.h>
#include <sec21/strong_type.h>

#include <ostream>

namespace sec21
{
   template <typename T>
   struct printable : crtp<T, printable>
   {
      template <typename CharT, typename Traits>
      void print(std::basic_ostream<CharT, Traits>& os) const { os << this->self().get(); }
   };

   template <
      typename CharT, 
      typename Traits, 
      typename T, 
      typename Tag, 
      template<typename> typename... Policies>
   std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, strong_type<T, Tag, Policies...> const& object)
   {
      object.print(os);
      return os;
   }
}
