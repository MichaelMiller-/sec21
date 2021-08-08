#include <catch.hpp>

#include <sec21/numeric/drop.h>
#include <sec21/numeric/make_matrix.h>
#include <sec21/numeric/make_vector.h>
#include <sec21/numeric/ublas_allocator_wrapper.h>

TEST_CASE("drop row from matrix", "[sec21][numeric]")
{
   using namespace sec21;

   auto v1 = std::vector{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
   using allocator_t = numeric::ublas_allocator_wrapper<decltype(v1)::allocator_type>;
   auto input = numeric::make_matrix<allocator_t>(4, 4, begin(v1), end(v1));

   auto result = numeric::drop(input, {row{2}});

   decltype(v1) tmp(result.size1() * result.size2(), 0);
   auto result_data = result.data();
   std::copy(begin(result_data), end(result_data), begin(tmp));

   const auto expected = std::vector{0, 1, 2, 3, 4, 5, 6, 7, 12, 13, 14, 15};
   REQUIRE(std::equal(begin(expected), end(expected), begin(tmp)));
}
TEST_CASE("drop row and col from matrix", "[sec21][numeric]")
{
   using namespace sec21;

   auto v1 = std::vector{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
   using allocator_t = numeric::ublas_allocator_wrapper<decltype(v1)::allocator_type>;
   auto input = numeric::make_matrix<allocator_t>(4, 4, begin(v1), end(v1));

   auto result = numeric::drop(numeric::drop(input, {row{2}}), {col{1}});

   decltype(v1) tmp(result.size1() * result.size2(), 0);
   auto result_data = result.data();
   std::copy(begin(result_data), end(result_data), begin(tmp));

   const auto expected = std::vector{0, 2, 3, 4, 6, 7, 12, 14, 15};
   REQUIRE(std::equal(begin(expected), end(expected), begin(tmp)));
}
TEST_CASE("drop row from vector", "[sec21][numeric]")
{
   using namespace sec21;

   auto v1 = std::vector{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
   using allocator_t = numeric::ublas_allocator_wrapper<decltype(v1)::allocator_type>;
   auto input = numeric::make_vector<allocator_t>(begin(v1), end(v1));

   auto result = numeric::drop(input, {row{2}, row{4}, row{5}, row{13}});

   decltype(v1) tmp(result.size(), 0);
   auto result_data = result.data();
   std::copy(begin(result_data), end(result_data), begin(tmp));

   const auto expected = std::vector{0, 1, 3, 6, 7, 8, 9, 10, 11, 12, 14, 15};
   REQUIRE(std::size(result) == std::size(expected));
   REQUIRE(std::equal(begin(expected), end(expected), begin(tmp)));
}
TEST_CASE("drop row from a std::vector", "[sec21][numeric]")
{
   using namespace sec21;

   auto input = std::vector{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
   auto result = numeric::drop(input, {row{2}, row{4}, row{5}, row{13}});
   const auto expected = std::vector{0, 1, 3, 6, 7, 8, 9, 10, 11, 12, 14, 15};

   REQUIRE(std::size(result) == std::size(expected));
   REQUIRE(std::equal(begin(expected), end(expected), begin(result)));
}