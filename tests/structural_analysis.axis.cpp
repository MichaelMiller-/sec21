#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <sec21/structural_analysis/axis.h>

using namespace sec21::structural_analysis;

static_assert(static_cast<int>(Axis::X) == 0, "Value of X-Axis has to be 0");
static_assert(static_cast<int>(Axis::Y) == 1, "Value of Y-Axis has to be 1");
static_assert(static_cast<int>(Axis::Z) == 2, "Value of Z-Axis has to be 2");

TEST_CASE("key->axis->value table", "[core]")
{
   using namespace sec21::structural_analysis;

   key_axis_value_table<int, int> l;

   auto [it1, b1] = l.insert_or_assign(1, {42, 12});
   REQUIRE(b1 == true);

   REQUIRE(l[1][Axis::X] == 42);
   REQUIRE(l[1][Axis::Y] == 12);

   auto [it2, b2] = l.insert_or_assign(1, {7, 33});
   REQUIRE(b2 == false);
   REQUIRE(l[1][Axis::X] == 7);
   REQUIRE(l[1][Axis::Y] == 33);

   REQUIRE_THROWS(l[2][Axis::X] == 42);

   auto [it3, b3] = l.insert_or_assign(3, {1, 2});
   REQUIRE(b3 == true);
   REQUIRE(l[3][Axis::X] == 1);
   REQUIRE(l[3][Axis::Y] == 2);

   l[3][Axis::Y] = 6;
   REQUIRE(l[3][Axis::Y] == 6);

   SECTION("fill")
   {
      key_axis_value_table<int, int> l1;

      std::vector in{ 1, 2, 4 };
      l1.fill(std::begin(in), std::end(in));

      REQUIRE(l1[1][Axis::X] == 0);
      REQUIRE(l1[1][Axis::Y] == 0);
      REQUIRE(l1[2][Axis::X] == 0);
      REQUIRE(l1[2][Axis::Y] == 0);
      REQUIRE_THROWS(l1[3][Axis::X] == 42);
      REQUIRE_THROWS(l1[3][Axis::Y] == 42);
      REQUIRE(l1[4][Axis::X] == 0);
      REQUIRE(l1[4][Axis::Y] == 0);
      REQUIRE_THROWS(l[5][Axis::X] == 42);
      REQUIRE_THROWS(l[5][Axis::Y] == 42);
   }
}

//! \todo benchmark :: vs v2
namespace v2
{
   template <typename T>
   class proxy_axis
   {
      T& value;
   public:
      explicit proxy_axis(T& t) : value{t} {}

      auto operator[](Axis a)
      {
         return value[static_cast<std::size_t>(a)];
      }
   };

   // key -> axis -> value (std::map variante)
   template <typename Key, typename Value, int Dimension = 2>
   struct key_axis_value_table
   {
      using key_t = Key;
      using value_t = std::array<Value, Dimension>;
      
      std::map<key_t, value_t> keys;

      auto insert_or_assign(key_t key, value_t&& v)
      {
         return keys.insert({key, v});
      }

      auto operator[](key_t key) -> proxy_axis<Value>
      {
         if (auto it = keys.find(key); it != std::end(keys))
            return proxy_axis{it->second};
         throw std::invalid_argument("invalid key");
      }
   };

}