#pragma once

#include <sec21/database/column.h>
#include <sec21/database/column_type.h>
#include <sec21/database/table.h>

#include <fmt/format.h>

#include <iomanip>
#include <sstream>
#include <string_view>
#include <tuple>
#include <type_traits>

namespace sec21::database
{
   namespace detail
   {
      template <typename Column>
      void get_column_type(std::ostream& os) noexcept
      {
         os << column_type<typename Column::value_t>::name;
      }

      template <typename Constraints, std::size_t... Is>
      void concat_constraints(std::ostream& os, std::index_sequence<Is...>) noexcept
      {
         // ((os << (Is == 0 ? "" : " ") << std::tuple_element_t<Is, Constraints>::value), ...);
         ((os << " " << std::tuple_element_t<Is, Constraints>::value), ...);
      }

      template <typename Column>
      void column_constraints(std::ostream& os) noexcept
      {
         using constraints_t = typename Column::constraints_t;
         constexpr auto indices = std::make_index_sequence<std::tuple_size_v<constraints_t>>{};
         concat_constraints<constraints_t>(os, indices);
      }

      template <typename Columns, std::size_t... Is>
      void column_table_description(std::ostream& os, std::index_sequence<Is...>) noexcept
      {
         // clang-format off
         os << '(';
         (
            (os << (Is == 0 ? "" : ","), 
            os << std::tuple_element_t<Is, Columns>::name << " ",
            detail::get_column_type<std::tuple_element_t<Is, Columns>>(os),
            detail::column_constraints<std::tuple_element_t<Is, Columns>>(os)
         ), ...);
         os << ");";
         // clang-format on
      }

      template <typename Columns, std::size_t... Is>
      void column_names(std::ostream& os, std::index_sequence<Is...>) noexcept
      {
         ((os << (Is == 0 ? "" : ",") << std::tuple_element_t<Is, Columns>::name), ...);
      }

      template <typename Columns, std::size_t... Is>
      void embraced_column_names(std::ostream& os, std::index_sequence<Is...>) noexcept
      {
         os << '(';
         column_names<Columns>(os, std::make_index_sequence<sizeof...(Is)>{});
         os << ')';
      }

      template <typename Column, typename Row>
      void escape_value_if_necessary(std::ostream& os, Row const& row) noexcept
      {
         if constexpr (std::is_convertible_v<typename Column::value_t, std::string>) {
            os << std::quoted(Column::get(row), '\'');
            return;
         }
         os << Column::get(row);
      }

      template <typename Columns, typename Row, std::size_t... Is>
      void embraced_row_values(std::ostream& os, Row const& row, std::index_sequence<Is...>) noexcept
      {
         os << '(';
         ((os << (Is == 0 ? "" : ","), escape_value_if_necessary<std::tuple_element_t<Is, Columns>>(os, row)), ...);
         os << ')';
      }
   } // namespace detail

   template <typename Table>
   auto create_table()
   {
      using reflection_t = typename table<Table>::metainfo;
      constexpr auto indices = std::make_index_sequence<std::tuple_size_v<reflection_t>>{};

      std::stringstream out;
      out << "CREATE TABLE ";
      out << table<Table>::name;
      detail::column_table_description<reflection_t>(out, indices);
      return out.str();
   }

   //! \todo auto insert_into(RowConcept auto const& row)
   template <typename Row>
   auto insert_into(Row const& row)
   {
      using reflection_t = typename table<Row>::metainfo;
      constexpr auto indices = std::make_index_sequence<std::tuple_size_v<reflection_t>>{};

      std::stringstream out;
      out << "INSERT INTO ";
      out << table<Row>::name << " ";
      detail::embraced_column_names<reflection_t>(out, indices);
      out << " VALUES ";
      detail::embraced_row_values<reflection_t>(out, row, indices);
      out << ";";
      return out.str();
   }

   template <typename Table>
   auto select()
   {
      using reflection_t = typename table<Table>::metainfo;
      constexpr auto indices = std::make_index_sequence<std::tuple_size_v<reflection_t>>{};
#if 1
      std::stringstream out;
      out << "SELECT ";
      detail::column_names<reflection_t>(out, indices);
      out << " FROM ";
      out << table<Table>::name;
      return out.str();
#else
      return fmt::format("SELECT {} FROM {}", column_names<reflection_t>(), table<Table>::name);
#endif
   }
} // namespace sec21::database
