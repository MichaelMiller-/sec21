#pragma once

#include <cmath>
#include <format>
#include <utility>

namespace sec21
{
   struct memory
   {
      using value_t = unsigned long long;
      value_t bytes{};

      friend auto operator<=>(memory const&, memory const&) = default;

      friend auto operator<<(std::ostream& out, memory const& obj) -> auto& { return out << obj.bytes; }
   };

   [[nodiscard]] constexpr auto operator+(memory const& lhs, std::byte rhs) noexcept -> memory
   {
#if __cpp_lib_to_underlying >= 202102L
      return {lhs.bytes + std::to_underlying(rhs)};
#else
      return {lhs.bytes + static_cast<std::underlying_type_t<decltype(rhs)>>(rhs)};
#endif
   }

   [[nodiscard]] constexpr auto operator+(memory const& lhs, memory const& rhs) noexcept -> memory
   {
      return {lhs.bytes + rhs.bytes};
   }

   [[nodiscard]] constexpr auto operator-(memory const& lhs, std::byte rhs) -> memory
   {
#if __cpp_lib_to_underlying >= 202102L
      if (std::to_underlying(rhs) > lhs.bytes) {
         throw std::out_of_range{"right-hand-side is greater than the left-hand-side"};
      }
      return {lhs.bytes - std::to_underlying(rhs)};
#else
      if (static_cast<std::underlying_type_t<decltype(rhs)>>(rhs) > lhs.bytes) {
         throw std::out_of_range{"right-hand-side is greater than the left-hand-side"};
      }
      return {lhs.bytes - static_cast<std::underlying_type_t<decltype(rhs)>>(rhs)};
#endif
   }

   [[nodiscard]] constexpr auto operator-(memory const& lhs, memory const& rhs) -> memory
   {
      if (rhs.bytes > lhs.bytes) {
         throw std::out_of_range{"right-hand-side is greater than the left-hand-side"};
      }
      return {lhs.bytes - rhs.bytes};
   }

   [[nodiscard]] constexpr auto operator*(memory const& lhs, std::byte rhs) noexcept -> memory
   {
#if __cpp_lib_to_underlying >= 202102L
      return {lhs.bytes * std::to_underlying(rhs)};
#else
      return {lhs.bytes * static_cast<std::underlying_type_t<decltype(rhs)>>(rhs)};
#endif
   }

   [[nodiscard]] constexpr auto operator*(memory const& lhs, memory const& rhs) noexcept -> memory
   {
      return {lhs.bytes * rhs.bytes};
   }
} // namespace sec21

template <>
struct std::formatter<sec21::memory>
{
   constexpr auto parse(std::format_parse_context& ctx)
   {
      auto pos = ctx.begin();
      while (pos != ctx.end() and *pos != '}') {
         if (*pos == 'h' or *pos == 'H') {
            human_readable = true;
         }
         if (*pos == '.') {
            pos = std::from_chars(++pos, ctx.end(), precision).ptr;
            --pos;
         }
         ++pos;
      }
      return pos;
   }

   auto format(sec21::memory const& obj, std::format_context& ctx) const
   {
      constexpr auto units = std::array{"B", "kB", "MB", "GB", "TB", "PB"};

      if (human_readable) {
         constexpr auto factor = 1000;
         //! \todo constexpr auto factorIEC = 1024;

         auto value = static_cast<double>(obj.bytes);
         auto unit = units[0];

         for (decltype(units.size()) i = 0; i < units.size(); ++i) {
            if (obj.bytes >= std::pow(factor, i)) {
               value = static_cast<double>(obj.bytes) / std::pow(factor, i);
               unit = units[i];
            }
         }
         return std::format_to(ctx.out(), "{:.{}f}{}", value, precision, unit);
      }
      return std::format_to(ctx.out(), "{}{}", obj.bytes, units[0]);
   }

   bool human_readable{false};
   std::size_t precision{2};
};
