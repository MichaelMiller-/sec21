#include <catch.hpp>

#include <cstdint>
#include <stdexcept>
#include <type_traits>

namespace sec21
{
   class bit_pattern
   {
      std::uint64_t expected{};
      std::uint64_t mask{ 0xFFFFFFFFFFFFFFFF };

   public:
      template <std::size_t Size>
#ifdef __cpp_concepts
      requires (Size > 2)
#endif
#ifdef __cpp_consteval
      explicit consteval bit_pattern(const char (&input)[Size])
#else
      explicit bit_pattern(const char (&input)[Size])
#endif
      {
         static_assert(Size > 2);

         std::uint64_t current_bit = (1 << (Size - 2));

         for (const char value : input)
         {
            if (value == 0)
               return;

            if (value == '1') {
               expected |= current_bit;
            }
            else if (value == '0') {}
            else if (value == 'x' || value == 'X') {
               mask &= ~current_bit;
            }
            else {
               throw std::logic_error("Unkown character");
            }
            current_bit >>= 1;
         }
      }

#ifdef __cpp_concepts
      template <std::unsigned_integral T>
#else
      template <typename T>
#endif      
      constexpr friend bool operator == (T const& value , bit_pattern const& pattern)
      {
         return (value & pattern.mask) == pattern.expected;
      }
   };
}

TEST_CASE("match bits via a pattern", "[sec21][core]")
{
   using namespace sec21;

#ifdef __cpp_consteval
   STATIC_REQUIRE(0b1111111u == bit_pattern("1111111"));
   STATIC_REQUIRE(0b1101010u == bit_pattern("11xxx10"));
   STATIC_REQUIRE(0b1111111u == bit_pattern("1111111"));
   STATIC_REQUIRE(0b0000000u == bit_pattern("0000000"));
   STATIC_REQUIRE(0b1101010u == bit_pattern("xxxxxxx"));
   STATIC_REQUIRE(0b1101010u == bit_pattern("xxx10xx"));
#else
   REQUIRE(0b1111111u == bit_pattern("1111111"));
   REQUIRE(0b1101010u == bit_pattern("11xxx10"));
   REQUIRE(0b1111111u == bit_pattern("1111111"));
   REQUIRE(0b0000000u == bit_pattern("0000000"));
   REQUIRE(0b1101010u == bit_pattern("xxxxxxx"));
   REQUIRE(0b1101010u == bit_pattern("xxx10xx"));
#endif
}
