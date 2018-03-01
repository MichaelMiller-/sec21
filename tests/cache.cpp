#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include "sec21/policies/clear.h"

TEST_CASE("test clear N entries", "[cache][policies]")
{
   using namespace sec21;

   std::vector<int> result{ 1, 2, 3 };
   using policy = policies::clear_after_N_entries<5>;

   policy::apply(result);
   REQUIRE(result.size() == 3);

   result.push_back(4);
   policy::apply(result);
   REQUIRE(result.size() == 4);

   result.push_back(5);
   policy::apply(result);
   REQUIRE(result.size() == 5);

   result.push_back(6);
   policy::apply(result);
   REQUIRE(result.size() == 0);

   result.push_back(7);
   policy::apply(result);
   REQUIRE(result.size() == 1);
}

#include "sec21/cache.h"

int real_function(std::string key)
{
   // really time consuming calculation 
   if (key == "key1")
      return 42;
   if (key == "key2")
      return 7;
   if (key == "key3")
      return 1;
   return 23;
}

TEST_CASE("test cache functionality (cache hits not considered)", "[cache]")
{
   using namespace sec21;

   using policy = policies::clear_after_N_entries<5>;
   auto f = make_cached_function<policy>(real_function);

   REQUIRE(f("key1") == 42);
   REQUIRE(f("key2") == 7);
   REQUIRE(f("key3") == 1);
   REQUIRE(f("unkown key") == 23);
}