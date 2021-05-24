#pragma once

#include <sec21/crtp.h>

namespace sec21
{
   template <typename T>
   [[deprecated]] struct addable : crtp<T, addable>
   {
      T operator + (T const& other)  { return T{ this->self() + other }; }
   };
}
