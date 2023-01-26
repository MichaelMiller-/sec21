#pragma once

#include <stdexcept>
#include <type_traits>

namespace sec21
{
   class bit_pattern
   {
      std::uint64_t expected{};
      std::uint64_t mask{0xFFFFFFFFFFFFFFFF};

    public:
      template <std::size_t Size>
#if __cpp_concepts >= 201907L
         requires(Size > 2)
#endif
#if __cpp_consteval >= 201811L
      explicit consteval bit_pattern(const char (&input)[Size])
#else
      explicit bit_pattern(const char (&input)[Size])
#endif
      {
         static_assert(Size > 2);

         std::uint64_t current_bit = (1 << (Size - 2));

         for (const char value : input) {
            if (value == 0) {
               return;
            }

            if (value == '1') {
               expected |= current_bit;
            } else if (value == '0') {
            } else if (value == 'x' || value == 'X') {
               mask &= ~current_bit;
            } else {
               throw std::logic_error("Unknown character");
            }
            current_bit >>= 1;
         }
      }

#if __cpp_concepts >= 201907L
      template <std::unsigned_integral T>
#else
      template <typename T>
#endif
      constexpr friend bool operator==(T const& value, bit_pattern const& pattern)
      {
         return (value & pattern.mask) == pattern.expected;
      }
   };
} // namespace sec21
