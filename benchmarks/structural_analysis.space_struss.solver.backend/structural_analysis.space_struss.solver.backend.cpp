#include "generated_boilerplate.h"

#include <sec21/numeric/make_vector.h>
#include <sec21/numeric/make_matrix.h>
#include <sec21/structural_analysis/solver/backend/viennacl.h>
#include <sec21/structural_analysis/solver/backend/eigen.h>

#include <vector>

int main() 
{
   using namespace sec21::structural_analysis::solver::backend;

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

   benchmark_runner(
      nonius::benchmark("viennacl: displacement() with boost::ublas input", [&] { 
         return viennacl_impl::displacement(K, F);
      }),
      nonius::benchmark("Eigen: displacement() with boost::ublas input", [&] { 
         return eigen::displacement(K, F);
      }),
      nonius::benchmark("Eigen: displacement() with std::vector input", [&] { 
         return eigen::displacement(K, F);
      })      
   );

   return 0;
}
