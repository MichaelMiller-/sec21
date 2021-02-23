#pragma once

#include <sec21/policy/addition.h>
#include <sec21/policy/subtraction.h>
#include <sec21/policy/multiplication.h>
#include <sec21/policy/division.h>
#include <sec21/policy/increment.h>
#include <sec21/policy/decrement.h>

namespace sec21::policy
{
   template <typename T>
   struct integer_arithmetic : addition<T>,
                               subtraction<T>,
                               multiplication<T>,
                               division<T>,
                               increment<T>,
                               decrement<T>
   { };
}
