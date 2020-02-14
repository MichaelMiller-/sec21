#define CATCH_CONFIG_MAIN
#include "catch.hpp"

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

   // template <typename CompareOperator, typename LogicOperator, typename... Ts>
   // struct generic_logic : std::tuple<Ts...>
   // {
   //     template <typename U>
   //     constexpr bool operator ()(U const& u) const noexcept
   //     {
   //         // LogicOperator(CompareOperator(v, u), ...)
   //         return std::apply([&](auto const& ... v) { return (CompareOperator(v, u) || ...);}, get());
   //     }
   // private:
   //     constexpr std::tuple<Ts...> const& get() const noexcept
   //     {
   //         return *this;
   //     }
   // };
}

TEST_CASE("syntax helper", "[sec21][compilation-only]")
{
   using namespace sec21;

   constexpr int x{10};
   constexpr int y{50};
   constexpr int z{99};

   static_assert(any_of{x, y, z} < 100);
   static_assert(any_of{x, y, z} < 50);
   static_assert((any_of{x, y, z} < 10) == false);
   static_assert((any_of{x, y, z} < 9) == false);
   static_assert((any_of{x, y, z} < 1) == false);

   SUCCEED("Nothing to test. Compiletime-Tests");
}
