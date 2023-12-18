#include <catch.hpp>

#include <sec21/blob.h>

struct foo
{
   int x{0};
   int y{0};
   double d{};
   float f{};
   std::array<int, 8> a{};
};

TEST_CASE("memory blob", "[sec21][core]")
{
   const foo obj{17, 42, 3.14, 4.2f, {1, 2, 4, 5, 6, 7, 8, 9}};

   SECTION("store object")
   {
      const auto blob = sec21::blob{obj};
      REQUIRE(blob.size() == sizeof(foo));
   }
   SECTION("convert blob")
   {
      const auto blob = sec21::blob{obj};
      REQUIRE(blob.size() == sizeof(foo));

      foo result = blob;
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
   SECTION("retrieve the first int")
   {
      const auto blob = sec21::blob{obj};
      int i = blob;
      REQUIRE(i == 17);
   }
}
