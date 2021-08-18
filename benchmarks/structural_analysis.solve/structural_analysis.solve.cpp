#include "generated_boilerplate.h"

#include <sec21/structural_analysis/solve.h>
#include <sec21/structural_analysis/node.h>
#include <sec21/structural_analysis/member.h>
#include <sec21/structural_analysis/space_truss.h>
#include <sec21/structural_analysis/system_result.h>
#include <sec21/structural_analysis/solver/backend/viennacl.h>
#include <sec21/structural_analysis/solver/backend/eigen.h>

int main() 
{
   using namespace sec21;
   using namespace sec21::structural_analysis;
   using namespace sec21::structural_analysis::solver::backend;
   using namespace sec21::units::literals;

   using member_t = member<int, double>;
   using node_t = node<2, int, double>;
   using space_truss_t = space_truss<node_t, member_t>;

   auto sys = space_truss_t{};

   using node_t = decltype(sys)::node_t;
   using support_t = node_t::global_support_t;
   using loadcase_t = loadcase<decltype(sys)>;

   auto n1 = add_node(sys, {1, {0.0, 3.0}});
   auto n2 = add_node(sys, {2, {3.0, 3.0}});
   auto n3 = add_node(sys, {3, {3.0, 0.0}, support_t{false, true}});
   auto n4 = add_node(sys, {4, {0.0, 0.0}, support_t{true, true}});

   //! \todo use units
   // constexpr auto E = 210.0_kPa;
   // constexpr auto A = unit::square_meter<double>(0.004);
   auto m1 = add_member(sys, 1, 2, {1, 0.004, 21'000'000});
   auto m2 = add_member(sys, 3, 2, {2, 0.004, 21'000'000});
   auto m3 = add_member(sys, 4, 3, {3, 0.004, 21'000'000});
   auto m4 = add_member(sys, 4, 1, {4, 0.004, 21'000'000});
   auto m5 = add_member(sys, 4, 2, {5, 0.004, 21'000'000});
   auto m6 = add_member(sys, 3, 1, {6, 0.004, 21'000'000});

   loadcase_t lf1;
   lf1.node_load.emplace_back(2, loadcase_t::load_t{{10.0_kN, -10.0_kN}});

   benchmark_runner(
      nonius::benchmark("ViennaCL solver backend", [&] { 
         const auto success = solve<solver::backend::viennacl_impl>(sys, lf1);
         return success.value();
      }),
      nonius::benchmark("Eigen solver backend", [&] {
         const auto success = solve<solver::backend::eigen>(sys, lf1);
         return success.value();
      })
   );

   return 0;
}
