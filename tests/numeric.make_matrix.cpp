#include <catch.hpp>

#include <sec21/numeric/make_matrix.h>
#include <sec21/numeric/ublas_allocator_wrapper.h>

#include <memory_resource>
#include <scoped_allocator>
#include <vector>

using namespace sec21::numeric;
//! \todo enable if boost::ublas can deal with std::allocator. boost uses c++20 deprecated methods()
#if 0
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
#endif
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
TEST_CASE("create a matrix with dimension and initial value and wrap allocator", "[sec21][numeric]") 
{
   using allocator_wrapper_t = ublas_allocator_wrapper<std::allocator<int>>;

   auto result = make_matrix<int, allocator_wrapper_t>(5, 3, {});
   REQUIRE(result.size1() * result.size2() == 15);
}
TEST_CASE("create a matrix from a std::vector and wrap allocator", "[sec21][numeric]") 
{
   const auto input = std::vector{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
   using alloc_t = decltype(input)::allocator_type;
   using allocator_wrapper_t = ublas_allocator_wrapper<alloc_t>;

   auto result = make_matrix<allocator_wrapper_t>(5, 3, begin(input), end(input));
   REQUIRE(result.size1() * result.size2() == std::size(input));
}
TEST_CASE("create a matrix from a std::pmr::vector with allocator wrapper", "[sec21][numeric]") 
{
   std::array<std::byte, 256> buffer; 
   std::pmr::monotonic_buffer_resource res(buffer.data(), size(buffer));
   std::pmr::vector<int> input{ { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }, &res };
   using alloc_t = decltype(input)::allocator_type;
   using allocator_wrapper_t = ublas_allocator_wrapper<alloc_t>;

   auto result = make_matrix<allocator_wrapper_t>(5, 3, begin(input), end(input));
   REQUIRE(result.size1() * result.size2() == std::size(input));
}
