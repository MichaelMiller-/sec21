#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include "log/entry.h"

using namespace sec21::log;

TEST_CASE("try to convert a string into Kind object", "[sec21][logger]")
{
   REQUIRE(to_kind("info") == Kind::Info);
   REQUIRE(to_kind("Info") == Kind::Info);
   REQUIRE(to_kind("Error") == Kind::Error);
   REQUIRE(to_kind("error") == Kind::Error);
   REQUIRE(to_kind("Debug") == Kind::Debug);
   REQUIRE(to_kind("Warning") == Kind::Warning);
   REQUIRE(to_kind("Metric") == Kind::Metric);
}

TEST_CASE("try to convert a vector of string to a vector of log entries", "[sec21][logger]")
{
   const auto input = std::vector<std::string>{
      "[2001-01-01 23:53:23.777] [info] log: 244125 message: Starting application sec21:esbp 0.0",
      "[2001-01-01 23:53:23.777] [debug] log: 244125 message: Write results into database",
      "[2001-01-01 23:53:23.777] [info] log: 244125 message: done"
   };
   const auto result = to_entries(begin(input), end(input));

   const auto expected = std::vector<entry>{
      { Kind::Info, 978393203777, 244125, "Starting application sec21:esbp 0.0" },
      { Kind::Debug, 978393203777, 244125, "Write results into database" },
      { Kind::Info, 978393203777, 244125, "done" }
   };
   REQUIRE(size(result) == 3);
   REQUIRE(std::equal(begin(result), end(result), begin(expected)));
}