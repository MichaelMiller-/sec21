#include <catch.hpp>
#include "approx_equal.h"

#include <vector>
#include <array>

#include <sec21/numeric/make_vector.h>
#include <sec21/numeric/make_matrix.h>
#include <sec21/structural_analysis/solver/backend/eigen.h>

template <typename T>
auto to_std_vector(Eigen::Matrix<T, -1, 1, 0, -1, 1> const& m) 
{
   return std::vector<T>{ m.data(), m.data() + m.cols() * m.rows() };
}

using namespace sec21::structural_analysis::solver::backend;

TEST_CASE("Eigen displacement() implementation", "[sec21][structural_analysis][solver][eigen]") 
{
   Eigen::VectorXd  F(5); 
   F << 0.0,0.0,10000.0,-10000.0,0.0;

   Eigen::MatrixXd K(5, 5);
   K << 37899.5,-9899.49,-28000,0.0,-9899.49,
         -9899.49, 37899.5,0.0, 0.0, 9899.49,
      -28000, 0.0, 37899.5, 9899.49, 0.0,
         0.0,     0.0, 9899.49, 37899.5,     0.0,
      -9899.49, 9899.49,     0.0,     0.0, 37899.5;

   const auto result = to_std_vector(eigen::displacement(K, F));
   const auto expected = std::array{0.862219, 0.178571, 1.04079, -0.535714, 0.178571};

   REQUIRE(approx_equal(begin(result), end(result), begin(expected), 0.001));
}
TEST_CASE("Eigen displacement() implementation with std::vector interface", "[sec21][structural_analysis][solver][eigen]") 
{
   const auto K = std::vector<double>{
      37899.5,-9899.49,-28000,0.0,-9899.49,
      -9899.49, 37899.5,0.0, 0.0, 9899.49,
      -28000, 0.0, 37899.5, 9899.49, 0.0,
      0.0,     0.0, 9899.49, 37899.5,     0.0,
      -9899.49, 9899.49,     0.0,     0.0, 37899.5
   };
   auto F = std::vector<double>{0.0,0.0,10000.0,-10000.0,0.0};   

   const auto result = eigen::displacement(K, F);
   const auto expected = std::array{0.862219, 0.178571, 1.04079, -0.535714, 0.178571};

   REQUIRE(approx_equal(begin(result), end(result), begin(expected), 0.001));
}
TEST_CASE("Eigen displacement() implementation with boost::ublas interface", "[sec21][structural_analysis][solver][vienacl]") 
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

   const auto K = sec21::numeric::make_matrix<allocator_t>(5, 5, begin(input_K), end(input_K));
   const auto F = sec21::numeric::make_vector<allocator_t>(begin(input_F), end(input_F));

   const auto result = eigen::displacement(K, F);
   const auto expected = std::array{0.862219, 0.178571, 1.04079, -0.535714, 0.178571};

   REQUIRE(size(result) == size(expected));
   REQUIRE(approx_equal(begin(result), end(result), begin(expected), 0.001));
}
TEST_CASE("Eigen support_reactions() implementation", "[sec21][structural_analysis][solver][eigen]") 
{
   using precision_t = double;

   const auto input_K = std::vector<precision_t>{
      9'800.0, -9'800.0, 0.0,  -28'000.0,  -9'800.0,
      0.0,   0.0, -9'800.0, -9'800.0, -28'000.0,
      0.0,  -28'000.0, -9'800.0, -9'800.0,  0.0
   };

   const auto input_F = std::vector<precision_t>{0.862219, 0.178571, 1.04079, -0.535714, 0.178571};

   const auto result = std::vector<double>{};
   const auto expected = std::array{ 20000.0, -10000.0, -10000.0 };

   REQUIRE(approx_equal(begin(result), end(result), begin(expected), 0.001));
}