#include <catch.hpp>

#include <sec21/strong_type.h>
#include <sec21/policy/integer_arithmetic.h>
#include <sec21/policy/compare.h>
#include <sec21/policy/print.h>

#include <sstream>

TEST_CASE("test strong types", "[sec21][core]") 
{
   using namespace sec21;

   using width_t = strong_type<
      int, 
      struct width_tag, 
      policy::integer_arithmetic, 
      policy::compare, 
      policy::print>;
   using T = width_t::underlying_t;

   static_assert(std::is_same_v<T, underlying_type<width_t>>);
   static_assert(is_strong_type<int>::value == false);
   static_assert(is_strong_type<width_t>::value == true);
   static_assert(is_strong_type_v<int> == false);
   static_assert(is_strong_type_v<width_t> == true);

   width_t a{ 4 };
   width_t b{ 5 };
   // width_t fail(3.14); == warning -> use curly brackets

   SECTION("addition policy")
   {
      auto c = a + b;
      static_assert(std::is_same_v<decltype(c), width_t>);
      REQUIRE(static_cast<T>(c) == 9);

      b += width_t{ 15 };
      REQUIRE(static_cast<T>(b) == 20);
   }
   SECTION("subtraction policy")
   {
      auto c = b - a;
      static_assert(std::is_same_v<decltype(c), width_t>);
      REQUIRE(static_cast<T>(c) == 1);

      b -= width_t{ 2 };
      REQUIRE(static_cast<T>(b) == 3);
   }
   SECTION("multiplication policy")
   {
      auto c = a * b;
      static_assert(std::is_same_v<decltype(c), width_t>);
      REQUIRE(static_cast<T>(c) == 20);

      b *= width_t{ 2 };
      REQUIRE(static_cast<T>(b) == 10);
   }
   SECTION("division policy")
   {
      auto c = width_t{ 10 } / b;
      static_assert(std::is_same_v<decltype(c), width_t>);
      REQUIRE(static_cast<T>(c) == 2);

      a /= width_t{ 2 };
      REQUIRE(static_cast<T>(a) == 2);
   }
   SECTION("increment")
   {
      auto c = a++;
      static_assert(std::is_same_v<decltype(c), width_t>);
      REQUIRE(static_cast<T>(a) == 5);
      REQUIRE(static_cast<T>(c) == 4);

      ++b;
      REQUIRE(static_cast<T>(b) == 6);
   }
   SECTION("deincrement")
   {
      auto c = a--;
      static_assert(std::is_same_v<decltype(c), width_t>);
      REQUIRE(static_cast<T>(a) == 3);
      REQUIRE(static_cast<T>(c) == 4);

      --b;
      REQUIRE(static_cast<T>(b) == 4);
   }
   SECTION("compare")
   {
      REQUIRE(a != b);
      REQUIRE((a + width_t{1}) == b);
   }
   SECTION("print")
   {
      std::stringstream ss;
      ss << a;
      REQUIRE(ss.str() == "4");
   }
}

using type1_t = sec21::strong_type<int, struct tag1>;

template <typename T>
struct mix_addition
{
   friend constexpr T& operator += (T& lhs, type1_t const& rhs) noexcept
   {
      using lhs_t = typename T::underlying_t;
      using rhs_t = type1_t::underlying_t;
      static_assert(std::is_same_v<lhs_t, rhs_t>);

      static_cast<lhs_t&>(lhs) += static_cast<rhs_t const&>(rhs);
      return lhs;
   }
   friend constexpr T operator + (T const& lhs, type1_t const& rhs) noexcept
   {
      using lhs_t = typename T::underlying_t;
      using rhs_t = type1_t::underlying_t;
      static_assert(std::is_same_v<lhs_t, rhs_t>);

      return T(static_cast<lhs_t const&>(lhs) + static_cast<rhs_t const&>(rhs));
   }
};
// also possible
//template <typename T>
//using mix_addition = sec21::policy::mixed_addition<T, OtherType>;

using type2_t = sec21::strong_type<int, struct tag2, mix_addition>;

TEST_CASE("mix strong types", "[sec21][core]")
{
   using namespace sec21;

   type1_t a{ 4 };
   type2_t b{ 5 };

   using T = type2_t::underlying_t;

   auto c = b + a;
   static_assert(std::is_same_v<decltype(c), type2_t>);
   REQUIRE(static_cast<T>(c) == 9);
}