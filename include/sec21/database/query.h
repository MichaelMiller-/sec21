#pragma once

#include <sec21/enclose.h>
#include <sec21/database/column.h>
#include <sec21/database/column_type.h>

#include <sstream>
#include <string_view>
#include <tuple>
#include <iomanip>
#include <type_traits>

namespace sec21::database
{
   template <typename T, typename... Columns> requires std::conjunction_v<is_column<Columns>...>
   auto select(std::tuple<Columns...> cols)
   {
      constexpr auto N = std::tuple_size_v<decltype(cols)>;
      static_assert(N > 0);

      const auto column_names = enclose(cols, [](auto const& v){ return v.name; }, ",", {}, {});
      return std::string{"SELECT "} + column_names + std::string{" FROM "} + table<T>::name;
   }

   template <typename T>
   auto select()
   {
      return select<T>(table<T>::metainfo());
   }

   template <typename T>
   auto create_table()
   {
      constexpr auto reflection = table<T>::metainfo();
      constexpr auto N = std::tuple_size_v<decltype(reflection)>;
      static_assert(N > 0);

      const auto column_names = enclose(reflection, [](auto v)
      {
        using value_t = typename decltype(v)::value_t;
        using ops_t = typename decltype(v)::ops_t;

        std::string ops = " ";
        if constexpr (std::tuple_size_v<ops_t> > 0) {
            ops += enclose(ops_t{}, [](auto op){ return decltype(op)::value; }, " ", {}, {});
        }
        return std::string{ v.name } + " " + column_type<value_t>::name + ops;
      }, ",", "(",  ")");

      return std::string{"CREATE TABLE "} + table<T>::name + column_names + ";";
   }

   template <typename T, typename... Args>
   auto insert(Args&&... args) noexcept
   {
      constexpr auto columns = table<T>::metainfo();
      constexpr auto N = std::tuple_size_v<decltype(columns)>;
      //! \todo static_assert same_types Args == columns::type_t

      const auto input = std::make_tuple(std::forward<Args>(args)...);
      const auto values = enclose(
         input,
         [](auto v) {
            std::stringstream ss{};
            if constexpr (std::is_convertible_v<decltype(v), std::string>) {
               ss << std::quoted(v, '\'');
            }
            else
               ss << v;
            return ss.str();
         }, ",", "(", ")");

      constexpr auto reflection = table<T>::metainfo();
      const auto column_names = enclose(reflection, [](auto const& v){ return v.name; }, ",", "(", ")");

      return std::string{"INSERT INTO "}
         + table<T>::name 
         + " "
         + column_names
         + " VALUES "
         + values
         + ";";
   }
}
