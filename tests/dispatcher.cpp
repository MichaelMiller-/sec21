#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <sec21/dispatcher.h>

int func1() { return 1; }
int func2() { return 2; }
int func3() { return 3; }
int ulimate_answer(int i) { return 42; }

TEST_CASE("simple test", "[dispatcher]")
{
   sec21::dispatcher<std::string, int> d;

   d.insert("call1", func1);
   d.insert("call2", func2);
   d.insert("call3", func3);
   d.insert("ulimate_answer", ulimate_answer);

   REQUIRE(d["call1"]() == 1);
   REQUIRE(d["call2"]() == 2);
   REQUIRE(d["call3"]() == 3);
   REQUIRE(d["ulimate_answer"](2) == 42);
}