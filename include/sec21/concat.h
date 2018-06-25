#pragma once

#include <sstream>
#include <string>

namespace sec21
{
   namespace detail 
   {
      template <typename CharT>
      void append2stream(std::basic_ostringstream<CharT> &) {}

      template <typename CharT, typename T, typename... Args>
      void append2stream(std::basic_ostringstream<CharT>& os, T&& value, Args&&... args)
      {
         os << std::forward<T>(value);
         append2stream(os, std::forward<Args>(args)...);
      }
   }

   //! \brief join string-convertible objects into one string
   // eg: std::string s = sec21::concat<char>("foo ", 42, " bar ", streamable_object, "value2");
   template <typename CharT, typename... Args>
   std::basic_string<CharT> concat(Args&&... args)
   {
      std::basic_ostringstream<CharT> os;
      detail::append2stream(os, std::forward<Args>(args)...);
      return os.str();
   }
}