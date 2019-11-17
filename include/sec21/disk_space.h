#pragma once

#include <sec21/strong_type.h>
#include <sec21/policies/comparable.h>
#include <sec21/policies/printable.h>

namespace sec21
{
   using disk_space_t = strong_type<unsigned long long, struct disk_space_in_byte, comparable, printable>;

   inline namespace literals
   {
      constexpr auto operator "" _B(unsigned long long v) noexcept { 
         return disk_space_t{ v }; 
      }

      constexpr auto operator "" _kB(unsigned long long v) noexcept { 
         return disk_space_t{ v * 1024 }; 
      }

      constexpr auto operator "" _MB(unsigned long long v) noexcept { 
         return disk_space_t{ v * 1024 * 1024 }; 
      }
      
      constexpr auto operator "" _GB(unsigned long long v) noexcept { 
         return disk_space_t{ v * 1024 * 1024 * 1024 }; 
      }

      constexpr auto operator "" _TB(unsigned long long v) noexcept { 
         return disk_space_t{ v * 1024 * 1024 * 1024 * 1024 }; 
      }
   }
}
