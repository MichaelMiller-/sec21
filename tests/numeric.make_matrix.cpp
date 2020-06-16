#include <catch.hpp>

#include <sec21/numeric/make_matrix.h>
#include <sec21/numeric/ublas_allocator_wrapper.h>

#include <memory_resource>
#include <scoped_allocator>
#include <vector>

using namespace sec21::numeric;

TEST_CASE("create a matrix from a std::vector", "[sec21][numeric]") 
{
   const auto input = std::vector{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
   auto result = make_matrix<decltype(input)::allocator_type>(5, 3, begin(input), end(input));

   REQUIRE(result.size1() * result.size2() == std::size(input));
}

TEST_CASE("create a matrix from size and initial value", "[sec21][numeric]") 
{
   auto result = make_matrix<int, std::allocator<int>>(5, 3, 1);
   REQUIRE(result.size1() * result.size2() == 15);
}
#if 0
TEST_CASE("create a matrix from a std::pmr::vector", "[sec21][numeric]") 
{
   using namespace sec21::numeric;

   std::array<std::byte, 256> buffer; 
   std::pmr::monotonic_buffer_resource res(buffer.data(), size(buffer));
   std::pmr::vector<int> input{ { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }, &res };

   auto result = make_matrix<decltype(input)::allocator_type>(5, 3, begin(input), end(input));

   REQUIRE(result.size1() * result.size2() == std::size(input));
}
#endif
TEST_CASE("create a matrix from a std::pmr::vector with allocator wrapper", "[sec21][numeric]") 
{
   std::array<std::byte, 256> buffer; 
   std::pmr::monotonic_buffer_resource res(buffer.data(), size(buffer));
   std::pmr::vector<int> input{ { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }, &res };

   using alloc_t = decltype(input)::allocator_type;
   auto result = make_matrix<ublas_allocator_wrapper<alloc_t>>(5, 3, begin(input), end(input));
   REQUIRE(result.size1() * result.size2() == std::size(input));
}
