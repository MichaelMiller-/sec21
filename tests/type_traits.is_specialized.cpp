#include <catch.hpp>

#include <sec21/type_traits/is_specialized.h>

namespace ns
{
   template <typename T>
   struct foo;

   template <>
   struct foo<int>
   {
   };

   template <>
   struct foo<std::string>
   {
   };
} // namespace ns

TEST_CASE("checks if a specialisation of a template class is available", "[sec21][type_traits]")
{
   using namespace sec21::type_traits;
   STATIC_REQUIRE(is_specialized_v<ns::foo, int>);
   STATIC_REQUIRE(is_specialized_v<ns::foo, double> == false);
   STATIC_REQUIRE(is_specialized_v<ns::foo, float> == false);
   STATIC_REQUIRE(is_specialized_v<ns::foo, std::string> == true);
   STATIC_REQUIRE(is_specialized_v<ns::foo, std::wstring> == false);
}