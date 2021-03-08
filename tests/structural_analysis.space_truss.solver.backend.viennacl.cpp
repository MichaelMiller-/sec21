#include <catch.hpp>
#include "approx_equal.h"

#include <vector>
#include <array>

#include <sec21/numeric/make_vector.h>
#include <sec21/numeric/make_matrix.h>
#include <sec21/structural_analysis/solver/backend/viennacl.h>

using namespace sec21::structural_analysis::solver::backend;
namespace numeric = sec21::numeric;

TEST_CASE("viennacl displacement() implementation", "[sec21][structural_analysis][solver][vienacl]") 
{
   using precision_t = double;

   const auto input_K = std::vector<precision_t>{
      37899.5,-9899.49,-28000,0.0,-9899.49,
         -9899.49, 37899.5,0.0, 0.0, 9899.49,
      -28000, 0.0, 37899.5, 9899.49, 0.0,
         0.0,     0.0, 9899.49, 37899.5,     0.0,
      -9899.49, 9899.49,     0.0,     0.0, 37899.5
   };
   const auto input_F = std::vector<precision_t>{0.0,0.0,10000.0,-10000.0,0.0};

   using allocator_t = decltype(input_F)::allocator_type;

   const auto K = numeric::make_matrix<allocator_t>(5, 5, begin(input_K), end(input_K));
   const auto F = numeric::make_vector<allocator_t>(begin(input_F), end(input_F));

   const auto result = viennacl_impl::displacement(K, F);
   const auto expected = std::array{0.862219, 0.178571, 1.04079, -0.535714, 0.178571};

   REQUIRE(size(result) == size(expected));
   REQUIRE(approx_equal(begin(result), end(result), begin(expected), 0.001));
}
TEST_CASE("viennacl support_reactions() implementation", "[sec21][structural_analysis][solver][vienacl]") 
{
   using precision_t = double;

   const auto input_K = std::vector<precision_t>{
      9'800.0, -9'800.0, 0.0,  -28'000.0,  -9'800.0,
      0.0,   0.0, -9'800.0, -9'800.0, -28'000.0,
      0.0,  -28'000.0, -9'800.0, -9'800.0,  0.0
   };

   const auto input_F = std::vector<precision_t>{0.862219, 0.178571, 1.04079, -0.535714, 0.178571};

   using allocator_t = decltype(input_F)::allocator_type;

   const auto K = numeric::make_matrix<allocator_t>(3, 5, begin(input_K), end(input_K));
   const auto F = numeric::make_vector<allocator_t>(begin(input_F), end(input_F));

   const auto result = viennacl_impl::support_reactions(K, F);
   const auto expected = std::array{ 20000.0, -10000.0, -10000.0 };

   REQUIRE(approx_equal(begin(result), end(result), begin(expected), 0.01));
}
