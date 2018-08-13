#pragma once

#include <sstream>
#include <string>

namespace sec21
{
   //! \brief join string-convertible objects into one string
   // eg: std::string s = sec21::concat<char>("foo ", 42, " bar ", streamable_object, "value2");
   template <typename CharT, typename... Args>
   std::basic_string<CharT> concat(Args&&... args)
   {
      std::basic_stringstream<CharT> ss;
      (void)std::initializer_list<int>{ (ss << args, 0)... };
      return ss.str();
   }
}