#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <tuple>
#include <functional>

namespace sec21
{
   template <typename... Ts>
   struct any_of : std::tuple<Ts...>
   {
      using std::tuple<Ts...>::tuple;

      template <typename U>
      constexpr bool operator<(U const &u) const noexcept
      {
         return std::apply([&](auto const &... v) { return ((v < u) || ...); }, get());
      }

   private:
      constexpr std::tuple<Ts...> const &get() const noexcept
      {
         return *this;
      }
   };

   template <typename... Ts>
   any_of(Ts &&...)->any_of<Ts...>;
}

TEST_CASE("syntax helper", "[sec21][core]")
{
   using namespace sec21;

   constexpr int x{10};
   constexpr int y{50};
   constexpr int z{99};

   STATIC_REQUIRE(any_of{x, y, z} < 100);
   STATIC_REQUIRE(any_of{x, y, z} < 50);
   STATIC_REQUIRE((any_of{x, y, z} < 10) == false);
   STATIC_REQUIRE((any_of{x, y, z} < 9) == false);
   STATIC_REQUIRE((any_of{x, y, z} < 1) == false);
}
