#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <sec21/reflection.h>
#include <sec21/serializer.h>
#include <sec21/archive/json.h>

#include <string>
#include <variant>

//! \todo https://www.bfilipek.com/2019/06/fsm-variant-game.html

struct Circling
{
  explicit Circling(const int startIndex)
    : mNextCirclePosIndex(startIndex)
  {
  }
  double mTimeSinceLastShot = 0.0;
  int mNextCirclePosIndex = 0;
};


struct FlyToCenter { };

struct ShootingFromCenter
{
  double mTimeSinceLastShot = 0.0;
  double mTimeSpentInCenter = 0;
};


struct FlyOut
{
  explicit FlyOut(const int cornerIndex)
    : mTargetCornerIndex(cornerIndex)
  {
  }

  int mTargetCornerIndex;
};

using states_t = std::variant<Circling, FlyToCenter, ShootingFromCenter, FlyOut>;

#include <sec21/visitor.h>

template <typename Variant, typename... Matchers>
auto match(Variant&& variant, Matchers&&... matchers)
{
   return std::visit(sec21::visitor_t{std::forward<Matchers>(matchers)...}, std::forward<Variant>(variant));
}

bool playerInOuterZone() {return false; }

#if 0
TEST_CASE("state machine", "[core]")
{
   states_t mState = Circling(1);

   mState = match(mState,
      [=](Circling& state) -> states_t
      {
         // implement circling logic here

         if (playerInOuterZone()) {
            // Switch to next state if applicable
            return FlyToCenter();
         }

         return state;
      },

      [=](const FlyToCenter&) -> states_t
      {
         // implement flying to center logic here
      },

      [=](ShootingFromCenter& state) -> states_t
      {
         // implement shooting from center logic here
         return FlyToCenter();

      },

      [=](const FlyOut& state) -> states_t
      {
         // implement flying out of center logic here
         return FlyToCenter();
      }
   );

   int z{ 7 };
   REQUIRE(z == 7);
}
#endif
