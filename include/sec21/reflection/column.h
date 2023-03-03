#pragma once

#include <sec21/fixed_string.h>
#include <sec21/type_traits/member_pointer_traits.h>

#include <string_view>
#include <tuple>

namespace sec21::reflection
{
   template <fixed_string Name, auto MemberPtr, typename... Constraints>
   struct column
   {
      static_assert(std::is_member_object_pointer_v<decltype(MemberPtr)>);

      using class_t = typename type_traits::member_pointer_traits<decltype(MemberPtr)>::class_t;
      using value_t = typename type_traits::member_pointer_traits<decltype(MemberPtr)>::value_t;
      using constraints_t = std::tuple<Constraints...>;

      static constexpr std::string_view name{Name};

      static auto& get(class_t& obj) noexcept { return obj.*MemberPtr; }
      static auto const& get(class_t const& obj) noexcept { return obj.*MemberPtr; }
   };
} // namespace sec21::reflection