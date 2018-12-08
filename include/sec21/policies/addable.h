#pragma once

#include <sec21/crtp.h>

namespace sec21
{
   template <typename T>
   struct addable : crtp<T, addable>
   {
      T operator + (T const& other)  { return T{ this->self() + other }; }
   };
}
