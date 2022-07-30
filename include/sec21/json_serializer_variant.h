#pragma once

#include <nlohmann/json.hpp>

#include <variant>

namespace sec21::detail
{
   template <int N>
   struct variant_switch
   {
      template <typename Variant>
      void operator()(int index, nlohmann::json const& value, Variant& v) const
      {
         if (index == N) {
            v = value.get<std::variant_alternative_t<static_cast<std::size_t>(N), Variant>>();
         } else
            variant_switch<N - 1>{}(index, value, v);
      }
   };

   template <>
   struct variant_switch<0>
   {
      template <typename Variant>
      void operator()(int index, nlohmann::json const& value, Variant& v) const
      {
         if (index == 0)
            v = value.get<std::variant_alternative_t<0, Variant>>();
         else {
            throw std::runtime_error("while converting json to variant: invalid index");
         }
      }
   };
} // namespace sec21::detail

namespace nlohmann
{
   template <typename... Args>
   struct adl_serializer<std::variant<Args...>>
   {
      static void to_json(json& j, std::variant<Args...> const& v)
      {
         std::visit(
            [&](auto&& value) {
               j["index"] = v.index();
               j["value"] = std::forward<decltype(value)>(value);
            },
            v);
      }

      static void from_json(json const& j, std::variant<Args...>& v)
      {
         const auto index = j.at("index").get<int>();
         sec21::detail::variant_switch<sizeof...(Args) - 1>{}(index, j.at("value"), v);
      }
   };
} // namespace nlohmann