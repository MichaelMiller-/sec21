#include <catch2/catch_test_macros.hpp>

#include <sec21/limited_quantity.h>

template <typename T, template <typename...> typename Operation>
struct op_wrapper
{
   constexpr auto operator()(auto lhs, auto rhs) const { return Operation{}(T{lhs}, T{rhs}); };
};

TEST_CASE("Limited quantity", "[sec21]") // NOLINT(*-function-cognitive-complexity)
{
   using value_t = sec21::limited_quantity<int, 1, 10>; // NOLINT(*-avoid-magic-numbers)

   SECTION("check for valid initialization")
   {
      auto const make_quantity = [](auto value) { [[maybe_unused]] value_t const obj{value}; };

      REQUIRE_THROWS(make_quantity(0));
      REQUIRE_NOTHROW(make_quantity(1));
      REQUIRE_NOTHROW(make_quantity(2));
      REQUIRE_NOTHROW(make_quantity(3));
      REQUIRE_NOTHROW(make_quantity(4));
      REQUIRE_NOTHROW(make_quantity(5));
      REQUIRE_NOTHROW(make_quantity(6));
      REQUIRE_NOTHROW(make_quantity(7));
      REQUIRE_NOTHROW(make_quantity(8));
      REQUIRE_NOTHROW(make_quantity(9));
      REQUIRE_NOTHROW(make_quantity(10));
      REQUIRE_THROWS(make_quantity(11));
   }
   SECTION("interval border")
   {
      STATIC_REQUIRE(value_t{}.min() == 1);
      STATIC_REQUIRE(value_t{}.max() == 10);
   }
   SECTION("increment")
   {
      value_t obj{9}; // NOLINT(*-avoid-magic-numbers)
      REQUIRE(obj.get() == 9);
      ++obj;
      REQUIRE(obj.get() == 10);
      REQUIRE_THROWS([&] { ++obj; }());
      REQUIRE(obj.get() == 10);
   }
   SECTION("decrement")
   {
      value_t obj{2}; // NOLINT(*-avoid-magic-numbers)
      REQUIRE(obj.get() == 2);
      --obj;
      REQUIRE(obj.get() == 1);
      REQUIRE_THROWS([&] { --obj; }());
      REQUIRE(obj.get() == 1);
   }
   SECTION("addition")
   {
      auto const op = op_wrapper<value_t, std::plus>{};
      REQUIRE(op(2, 3).get() == 5);
      REQUIRE(op(5, 5).get() == 10);
      REQUIRE(op(1, 1).get() == 2);
      REQUIRE_THROWS(op(1, 10));
      REQUIRE_THROWS(op(2, 9));
   }
   SECTION("subtraction")
   {
      auto const op = op_wrapper<value_t, std::minus>{};
      REQUIRE(op(4, 3).get() == 1);
      REQUIRE(op(6, 5).get() == 1);
      REQUIRE(op(10, 1).get() == 9);
      REQUIRE_THROWS(op(10, 11));
      REQUIRE_THROWS(op(1, 2));
      REQUIRE_THROWS(op(4, 12));
   }
   SECTION("multiplication")
   {
      auto const op = op_wrapper<value_t, std::multiplies>{};
      REQUIRE(op(4, 2).get() == 8);
      REQUIRE(op(2, 2).get() == 4);
      REQUIRE(op(10, 1).get() == 10);
      REQUIRE_THROWS(op(10, 11));
      REQUIRE_THROWS(op(9, 2));
      REQUIRE_THROWS(op(4, 12));
   }
   SECTION("division")
   {
      auto const op = op_wrapper<value_t, std::divides>{};
      REQUIRE(op(4, 3).get() == 1);
      REQUIRE(op(6, 5).get() == 1);
      REQUIRE(op(10, 2).get() == 5);
      REQUIRE_THROWS(op(10, 11));
      REQUIRE_THROWS(op(1, 2));
      REQUIRE_THROWS(op(4, 12));
   }
}