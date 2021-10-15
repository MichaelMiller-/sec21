#pragma once

#include <sec21/policy/compare.h>
#include <sec21/policy/print.h>
#include <sec21/strong_type.h>

namespace sec21
{
   using disk_space_t = strong_type<unsigned long long, struct disk_space_in_byte, policy::compare, policy::print>;

   inline namespace literals
   {
      constexpr auto operator"" _B(unsigned long long v) noexcept { return disk_space_t{v}; }

      constexpr auto operator"" _kB(unsigned long long v) noexcept { return disk_space_t{v * 1024}; }

      constexpr auto operator"" _MB(unsigned long long v) noexcept { return disk_space_t{v * 1024 * 1024}; }

      constexpr auto operator"" _GB(unsigned long long v) noexcept { return disk_space_t{v * 1024 * 1024 * 1024}; }

      constexpr auto operator"" _TB(unsigned long long v) noexcept
      {
         return disk_space_t{v * 1024 * 1024 * 1024 * 1024};
      }
   } // namespace literals
} // namespace sec21
