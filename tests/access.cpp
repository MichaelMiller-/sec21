#include <catch.hpp>

#include <sec21/access.h>

#include <array>

TEST_CASE("access a std::array", "[sec21][core]")
{
   using namespace sec21;

   SECTION("read")
   {
      std::array<int, 3> v{ 42, 17, 5 };

      REQUIRE(X(v) == 42);
      REQUIRE(Y(v) == 17);
      REQUIRE(Z(v) == 5);
   }
   SECTION("write")
   {
      std::array<int, 3> v{ 1, 2, 3 };

      REQUIRE(X(v) == 1);
      REQUIRE(Y(v) == 2);
      REQUIRE(Z(v) == 3);

      X(v) = 9;
      Y(v) = 12;
      Z(v) = 6;

      REQUIRE(X(v) == 9);
      REQUIRE(Y(v) == 12);
      REQUIRE(Z(v) == 6);
   }
}


struct vec2 
{
   int x{};
   int y{};
};

namespace sec21::customization_point
{
   template <>
   struct access_x<vec2>
   {
      auto operator()(vec2 const& t) const noexcept
      {
         return t.x;
      }
      auto& operator()(vec2& t) const noexcept
      {
         return t.x;
      }        
   };
   
   template <>
   struct access_y<vec2>
   {
      auto operator()(vec2 const& t) const noexcept
      {
         return t.y;
      }
      auto& operator()(vec2& t) const noexcept
      {
         return t.y;
      }        
   };    
}

TEST_CASE("access a usertype via customization point", "[sec21][core]")
{
   using namespace sec21;

   SECTION("read")
   {
      vec2 v{ 42, 17 };

      REQUIRE(X(v) == 42);
      REQUIRE(Y(v) == 17);
   }
   SECTION("write")
   {
      vec2 v{ 1, 2 };

      REQUIRE(X(v) == 1);
      REQUIRE(Y(v) == 2);

      X(v) = 9;
      Y(v) = 12;

      REQUIRE(X(v) == 9);
      REQUIRE(Y(v) == 12);
   }
}