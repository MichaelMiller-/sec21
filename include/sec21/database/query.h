#pragma once

#include <sec21/concat.h>
#include <sec21/database/column_type.h>
#include <sec21/database/constraints.h>
#include <sec21/reflection/table.h>

#include <iomanip>
#include <sstream>
#include <string_view>
#include <tuple>
#include <type_traits>

namespace sec21::database
{
   namespace detail
   {
      template <typename Columns>
      [[nodiscard]] auto column_table_description() noexcept
      {
         return '(' +
                concat(Columns{}, ",",
                       [](auto const& v) noexcept {
                          using T = std::remove_cvref_t<decltype(v)>;

                          auto constraints = concat_constraints<typename T::constraints_t>();

                          std::stringstream stream;
                          stream << v.name << " " << column_type<typename T::value_t>::value;
                          if (not empty(constraints)) {
                             stream << " " << constraints;
                          }
                          return stream.str();
                       }) +
                ");";
      }

      template <typename Columns>
      [[nodiscard]] auto concat_column_names() noexcept
      {
         return concat(Columns{}, ",", [](auto const& v) noexcept { return v.name; });
      }

      template <typename Columns>
      [[nodiscard]] auto embraced_column_names() noexcept
      {
         return '(' + concat_column_names<Columns>() + ')';
      }

      template <typename Column, typename Row>
      [[nodiscard]] auto escape_value_if_necessary(Row const& row) noexcept
      {
         std::stringstream stream;

         if constexpr (std::is_convertible_v<typename std::remove_cvref_t<Column>::value_t, std::string>) {
            stream << std::quoted(std::remove_cvref_t<Column>::get(row), '\'');
            return stream.str();
         }
         stream << std::remove_cvref_t<Column>::get(row);
         return stream.str();
      }

      template <typename Columns, typename Row>
      [[nodiscard]] auto embraced_row_values(Row const& row) noexcept
      {
         return '(' +
                concat(std::apply(
                   [&row](auto const&... value) {
                      return std::make_tuple(escape_value_if_necessary<decltype(value)>(row)...);
                   },
                   Columns{})) +
                ')';
      }
   } // namespace detail

   template <typename Table>
   auto create_table()
   {
      using reflection_t = typename reflection::table<Table>::metainfo;

      std::stringstream out;
      out << "CREATE TABLE " << reflection::table<Table>::name << detail::column_table_description<reflection_t>();
      return out.str();
   }

   template <typename Row>
   auto insert_into(Row const& row)
   {
      using reflection_t = typename reflection::table<Row>::metainfo;

      std::stringstream out;
      out << "INSERT INTO " << reflection::table<Row>::name << " " << detail::embraced_column_names<reflection_t>()
          << " VALUES " << detail::embraced_row_values<reflection_t>(row) << ";";
      return out.str();
   }

   template <typename Table>
   auto select()
   {
      using reflection_t = typename reflection::table<Table>::metainfo;

      std::stringstream out;
      out << "SELECT " << detail::concat_column_names<reflection_t>() << " FROM " << reflection::table<Table>::name;
      return out.str();
   }
} // namespace sec21::database
