#pragma once

#include <sec21/concat.h>

#include <string_view>

namespace sec21::database
{
   //! \brief PostgreSQL keyword
   struct serial
   {
      static constexpr std::string_view value = "SERIAL";
   };
   struct primary_key
   {
      static constexpr std::string_view value = "PRIMARY KEY";
   };
   struct not_null
   {
      static constexpr std::string_view value = "NOT NULL";
   };
   struct unique
   {
      static constexpr std::string_view value = "UNIQUE";
   };

   template <typename Constraints>
   [[nodiscard]] auto concat_constraints() noexcept
   {
      return concat(Constraints{}, " ", [](auto const& v) noexcept { return v.value; });
   }
} // namespace sec21::database