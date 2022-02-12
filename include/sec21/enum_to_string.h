#pragma once

#if __has_include(<experimental/reflect>)

#include <array>
#include <boost/algorithm/string/predicate.hpp>
#include <experimental/reflect>

namespace sec21
{
   namespace detail
   {
      namespace reflect = std::experimental::reflect;

      template <template <typename> typename Trait, std::experimental::reflect::ObjectSequence Sequence, auto... Is>
      consteval auto make_object_sequence_array(std::index_sequence<Is...>)
      {
         return std::array{Trait<std::experimental::reflect::get_element_t<Is, Sequence>>::value...};
      }

      template <typename T>
      requires reflect::Constant<T> && reflect::Named<T>
      struct constant_and_name
      {
         static constexpr auto value = std::pair{reflect::get_constant_v<T>, reflect::get_name_v<T>};
      };

      template <typename T>
      consteval auto enum_names() requires std::is_enum_v<T>
      {
         using reflected_enum_t = reflexpr(T);
         using enum_enumerators_t = reflect::get_enumerators_t<reflected_enum_t>;
         constexpr auto n = reflect::get_size_v<enum_enumerators_t>;
         using sequence = std::make_index_sequence<n>;
         return make_object_sequence_array<constant_and_name, enum_enumerators_t>(sequence{});
      }
   } // namespace detail

   template <typename T>
   constexpr auto to_string(T value) requires std::is_enum_v<T>
   {
      constexpr auto values = detail::enum_names<T>();
      const auto it = std::find_if(begin(values), end(values), [value](auto const& e) { return e.first == value; });
      return it->second;
   }

   template <typename T>
   constexpr auto to_enum(std::string_view value) requires std::is_enum_v<T>
   {
      constexpr auto values = detail::enum_names<T>();
      const auto it =
         std::find_if(begin(values), end(values), [value](auto const& e) { return boost::iequals(e.second, value); });
      if (it == end(values))
         throw std::runtime_error("cannot convert string to enum");
      return it->first;
   }
} // namespace sec21

#else

namespace sec21
{
   template <typename T>
   constexpr auto to_string(T);

   template <typename T>
   constexpr auto to_enum(std::string_view);

} // namespace sec21

#endif