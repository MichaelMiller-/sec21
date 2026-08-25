#pragma once

#include <type_traits>

namespace sec21::type_traits
{
   template <template <typename...> typename Base, typename U>
   struct is_instance_of : std::false_type
   {
   };

   template <template <typename...> typename Base, typename... Args>
   struct is_instance_of<Base, Base<Args...>> : std::true_type
   {
   };

   template <template <typename...> typename Base, typename U>
   inline constexpr bool is_instance_of_v = is_instance_of<Base, U>::value;
}

static_assert(is_instance_of_v<std::basic_string, std::string>, "must be a instance_of");
static_assert(is_instance_of_v<std::tuple, std::tuple<int, double>>, "must be a instance_of");
static_assert(!is_instance_of_v<std::tuple, std::vector<int>>, "should be false");
static_assert(!is_instance_of_v<std::vector, std::tuple<int, double>>, "should be false");

TEST_CASE("traits", "[sec21][traits]") { SUCCEED("Nothing to test. Compiletime-Tests"); }

// C++ Weekly - Ep 242 - Design By Introspection in C++20 (concepts + if constexpr)
// https://www.youtube.com/watch?v=sy32kAtsIKg
#if 0
auto allocated_size(auto const& container)
{
   if constexpr (requires { container.capacity(); }) {
      return container.capacity();
   }
   else {
      return container.size();
   }
}

vector::capacity();
array::size();

#endif

/*
template <typename T, typename=void>
struct is_variant : std::false_type {};

template <typename... Ts>
struct is_variant<std::variant<Ts...>> : std::true_type {};

*/