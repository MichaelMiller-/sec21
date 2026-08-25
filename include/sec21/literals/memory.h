#pragma once

#include <sec21/memory.h>

namespace sec21::literals
{
   //! \brief Byte
   constexpr auto operator""_B(unsigned long long n) noexcept -> memory { return {n}; }

   // SI
   //
   //! \brief Kilobyte
   constexpr auto operator""_kB(unsigned long long n) noexcept -> memory { return {n * 1000}; }
   //! \brief Megabyte
   constexpr auto operator""_MB(unsigned long long n) noexcept -> memory { return memory{n} * 1000_kB; }
   //! \brief Gigabyte
   constexpr auto operator""_GB(unsigned long long n) noexcept -> memory { return memory{n} * 1000_MB; }
   //! \brief Terabyte
   constexpr auto operator""_TB(unsigned long long n) noexcept -> memory { return memory{n} * 1000_GB; }
   //! \brief Petabyte
   constexpr auto operator""_PB(unsigned long long n) noexcept -> memory { return memory{n} * 1000_TB; }

   // IEC
   //
   //! \brief Kibibyte
   constexpr auto operator""_kiB(unsigned long long n) noexcept -> memory { return {n * 1024}; }
   //! \brief Mebibyte
   constexpr auto operator""_MiB(unsigned long long n) noexcept -> memory { return memory{n} * 1024_kiB; }
   //! \brief Gibibyte
   constexpr auto operator""_GiB(unsigned long long n) noexcept -> memory { return memory{n} * 1024_MiB; }
   //! \brief Tebibyte
   constexpr auto operator""_TiB(unsigned long long n) noexcept -> memory { return memory{n} * 1024_GiB; }
   //! \brief Pebibyte
   constexpr auto operator""_PiB(unsigned long long n) noexcept -> memory { return memory{n} * 1024_TiB; }
} // namespace sec21::literals
