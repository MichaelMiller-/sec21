#include <catch.hpp>

#include <iostream>

#include <sec21/blob.h>

struct foo
{
   int x{0};
   int y{0};
   double d{};
   float f{};
   std::array<int, 8> a{};
};

TEST_CASE("Memory blob", "[sec21][core]")
{
   foo const obj{17, 42, 3.14, 4.2f, {1, 2, 4, 5, 6, 7, 8, 9}};

   SECTION("Store object")
   {
      auto const blob = sec21::blob{obj};
      REQUIRE(blob.size() == sizeof(foo));
   }
   SECTION("Convert blob")
   {
      auto const blob = sec21::blob{obj};
      REQUIRE(blob.size() == sizeof(foo));

      foo const result = blob;
      REQUIRE(result.x == 17);
      REQUIRE(result.y == 42);
      REQUIRE(result.d == Catch::Approx(3.14));
      REQUIRE(result.f == Catch::Approx(4.2f));
      REQUIRE(result.a[0] == 1);
      REQUIRE(result.a[1] == 2);
      REQUIRE(result.a[2] == 4);
      REQUIRE(result.a[3] == 5);
      REQUIRE(result.a[4] == 6);
      REQUIRE(result.a[5] == 7);
      REQUIRE(result.a[6] == 8);
      REQUIRE(result.a[7] == 9);
   }
   SECTION("Retrieve the first int")
   {
      auto const blob = sec21::blob{obj};
      int i = blob;
      REQUIRE(i == 17);
   }
   SECTION("Store blob in a std::string")
   {
      auto const blob = sec21::blob{obj};
      auto const result = to_string(blob);
      // only test the blob size, stored in the first characters
      REQUIRE(result[0] == '5');
      REQUIRE(result[1] == '6');
   }
   SECTION("Parse blob from a std::string")
   {
      auto const input = sec21::blob{obj};
      auto const str = to_string(input);
      auto const mem = sec21::blob{sec21::blob::parse{}, str};

      auto const result = mem.as<foo>();
      REQUIRE(result.x == 17);
      REQUIRE(result.y == 42);
      REQUIRE(result.d == Catch::Approx(3.14));
      REQUIRE(result.f == Catch::Approx(4.2f));
      REQUIRE(result.a[0] == 1);
      REQUIRE(result.a[1] == 2);
      REQUIRE(result.a[2] == 4);
      REQUIRE(result.a[3] == 5);
      REQUIRE(result.a[4] == 6);
      REQUIRE(result.a[5] == 7);
      REQUIRE(result.a[6] == 8);
      REQUIRE(result.a[7] == 9);
   }
}
