#define _SILENCE_CXX17_UNCAUGHT_EXCEPTION_DEPRECATION_WARNING
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <sec21/combine.h>

// namespaces are only for convenience
namespace test_case_all_of
{
   constexpr auto lambda1 = [](auto e){ return e < 100; };
   constexpr auto lambda2 = [](auto e){ return e > 5; };
   constexpr auto lambda3 = [](auto e){ return e > 10; };

   auto f1 = sec21::all_of(lambda1, lambda2, lambda3);

   static_assert(f1(6) == false, "should be false");
   static_assert(f1(16) == true, "should be true");
   static_assert(f1(116) == false, "should be false");
}

namespace test_case_any_of
{
   constexpr auto lambda1 = [](auto e){ return e == 100; };
   constexpr auto lambda2 = [](auto e){ return e == 5; };
   constexpr auto lambda3 = [](auto e){ return e == 10; };

   auto f1 = sec21::any_of(lambda1, lambda2, lambda3);

   static_assert(f1(6) == false, "should be false");
   static_assert(f1(5) == true, "should be true");
   static_assert(f1(10) == true, "should be true");
   static_assert(f1(15) == false, "should be false");
   static_assert(f1(100) == true, "should be true");
   static_assert(f1(116) == false, "should be false");
}

TEST_CASE("combine", "[compilation-only]") {
   SUCCEED("Nothing to test. Compiletests");
}
