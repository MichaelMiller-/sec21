//! \remove
#define _SILENCE_CXX17_UNCAUGHT_EXCEPTION_DEPRECATION_WARNING
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <sec21/fixed_string.h>

//template <typename T, sec21::fixed_string Str>
//struct A {};
//using a_t = A<int, sec21::fixed_string("hello")>;

//! \todo write tests

TEST_CASE("fixed string", "[core]")
{
   using namespace sec21;
   sec21::fixed_string s("foo");
}
