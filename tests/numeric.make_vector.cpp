#include <catch.hpp>

#include <sec21/numeric/make_vector.h>
#include <sec21/numeric/ublas_allocator_wrapper.h>
#include <sec21/units.h>

#include <memory_resource>
#include <scoped_allocator>
#include <vector>

using namespace sec21::numeric;

TEST_CASE("create a vector from a std-sequence (vector)", "[sec21][numeric]") 
{
   const auto input = std::vector{ 1, 3, 4, 6, 8, 9, 10, 11, 2 };
   auto result = make_vector<int, std::allocator<int>>(std::begin(input), std::end(input));

   REQUIRE(std::size(result) == std::size(input));
   REQUIRE(std::equal(std::begin(result), std::end(result), std::begin(input), std::end(input)));
}
TEST_CASE("create a vector from a sequence with units", "[sec21][numeric]") 
{
   using namespace sec21::numeric;
   using namespace sec21::units::literals;
   using value_t = sec21::units::quantity<sec21::units::newton, int>;

   const auto input = std::vector<value_t>{ { 1_N }, { 2_kN }, { 422_N }, { 17_N } };
   auto result = make_vector<std::allocator<int>>(std::begin(input), std::end(input), [](auto&& q){ return q.value(); });

   const auto expected = std::vector{ 1, 2000, 422, 17 };
   REQUIRE(std::size(result) == std::size(expected));
   REQUIRE(std::equal(std::begin(result), std::end(result), std::begin(expected), std::end(expected)));
}

#if 0 // c++20
TEST_CASE("create a vector from a std::pmr::vector", "[sec21][numeric]") 
{
   using namespace sec21::numeric;

   std::array<std::byte, 256> buffer; 
   std::pmr::monotonic_buffer_resource res(buffer.data(), size(buffer));
   std::pmr::vector<int> input{ { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }, &res };

   auto result = make_vector<int, decltype(input)::allocator_type>(begin(input), end(input));
   REQUIRE(result.size() == std::size(input));
   REQUIRE(std::equal(std::begin(result), std::end(result), std::begin(input), std::end(input)));
}
#endif

TEST_CASE("create a vector from a std::pmr::vector with allocator wrapper", "[sec21][numeric]") 
{
   std::array<std::byte, 256> buffer; 
   std::pmr::monotonic_buffer_resource res(buffer.data(), size(buffer));
   std::pmr::vector<int> input{ { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }, &res };

   using alloc_t = decltype(input)::allocator_type;
   auto result = make_vector<int, ublas_allocator_wrapper<alloc_t>>(begin(input), end(input));
   REQUIRE(result.size() == std::size(input));
   REQUIRE(std::equal(std::begin(result), std::end(result), std::begin(input), std::end(input)));
}
#if 0
TEST_CASE("create a vector from a pmr::sequence with units", "[sec21][numeric]") 
{
   using namespace sec21::units::literals;
   using value_t = sec21::units::quantity<sec21::units::newton, int>;

   std::array<std::byte, 256> buffer; 
   std::pmr::monotonic_buffer_resource res(buffer.data(), size(buffer));
   std::pmr::vector<value_t> input{ { { 1_N }, { 2_kN }, { 422_N }, { 17_N } }, &res };

   using alloc_t = decltype(input)::allocator_type;
   auto result = make_vector<ublas_allocator_wrapper<alloc_t>>(std::begin(input), std::end(input), [](auto&& q){ return q.value(); });

   const auto expected = std::vector{ 1, 2000, 422, 17 };
   REQUIRE(std::size(result) == std::size(expected));
   REQUIRE(std::equal(std::begin(result), std::end(result), std::begin(expected), std::end(expected)));
}
#endif