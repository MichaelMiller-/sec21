#pragma once

namespace sec21::database
{
   template <typename T>
   struct column_type;

   template <typename T>
   [[nodiscard]] auto get_column_type() noexcept -> std::string_view
   {
      return column_type<T>::value;
   }
}