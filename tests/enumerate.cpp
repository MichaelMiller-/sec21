#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <vector>
#include <tuple>
#include <array>
#include <string>
#include <string_view>

#include <sec21/enumerate.h>

TEST_CASE("enumerate over array and get current value and index", "[enumerate]")
{
   const std::array<std::string, 3> input{ "foo", "bar", "baz" };

   constexpr std::array<size_t, 3> expected_indicies{ 0, 1, 2 };
   const std::array<std::string, 3> expected_values{ "foo", "bar", "baz" };

#ifdef __cpp_structured_bindings
   for (auto[i, value] : sec21::enumerate(input))
   {
      REQUIRE(i == expected_indicies[i]);
      REQUIRE(value == expected_values[i]);
   }
#endif
}

bool operator == (std::pair<int, std::string> const& lhs, std::pair<int, std::string> const& rhs) noexcept
{
   return lhs.first == rhs.first && lhs.second == rhs.second;
}

TEST_CASE("enumerate over map and get current value and index", "[enumerate]")
{
   const std::map<int, std::string> input{ 
      { 5, "foo" },
      { 6, "bar" },
      { 9, "baz" }
   };

   constexpr std::array<size_t, 3> expected_indicies{ 0, 1, 2 };
   const std::array<std::pair<int, std::string>, 3> expected_values{ {
      { 5, "foo" },
      { 6, "bar" },
      { 9, "baz" }
   } };

#ifdef __cpp_structured_bindings
   for (auto[i, value] : sec21::enumerate(input))
   {
      REQUIRE(i == expected_indicies[i]);
      //! \todo 
      // REQUIRE(value == expected_values[i]);
   }
#endif
}


namespace sec21::v2
{
   namespace detail
   {
      template <std::size_t... Is, typename F, typename... Ts>
      void enumerate_impl(std::index_sequence<Is...>, F&& f, Ts&&... ts)
      {
         (f(std::integral_constant<std::size_t, Is>{}, std::forward<Ts>(ts)), ...);
      }
   }

   template <typename F, typename... Ts>
   void enumerate(F&& f, Ts&&... ts)
   {
      v2::detail::enumerate_impl(std::index_sequence_for<Ts...>{}, std::forward<F>(f), std::forward<Ts>(ts)...);
   }
}
namespace sec21::v3
{
   namespace detail
   {
      template <std::size_t... Is, typename F, typename... Ts>
      void enumerate_impl(std::index_sequence<Is...>, F&& f, Ts&&... ts)
      {
         (f.template operator() < Is > (std::forward<Ts>(ts)), ...);
      }
   }

   template <typename F, typename... Ts>
   void enumerate(F&& f, Ts&&... ts)
   {
      v3::detail::enumerate_impl(std::index_sequence_for<Ts...>{}, std::forward<F>(f), std::forward<Ts>(ts)...);
   }
}

template <typename... Ts>
void print_with_index(Ts&&... ts)
{
   sec21::v3::enumerate([]<auto i>(auto x) {
      std::cout << i << ": " << x << std::endl;
   }, ts...);
}

namespace sec21::v4
{
   template <typename F, typename... Ts>
   void enumerate(F&& f, Ts&&... ts)
   {
      [&] <auto... Is>(std::index_sequence<Is...>) {
         (f.template operator() < Is > (std::forward<Ts>(ts)), ...);
      }(std::index_sequence_for<Ts...>{});
   }
}

//! \todo: write benchmarks