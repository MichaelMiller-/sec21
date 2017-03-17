#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include "sec21/structural_analysis/force.h"

#include <numeric>

#include <boost/qvm/vec_operations.hpp>

TEST_CASE("(resultierende) force", "[structural analysis]")
{
   using namespace sec21::structural_analysis;

   std::vector<force> forces = {
      { 1, 0 },
      { 0, 1 }
   };

   auto res = std::accumulate(forces.begin(), forces.end(), decltype(forces)::value_type{ 0, 0 });

   REQUIRE((res == force{ 1,1 }));
}