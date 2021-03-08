#include <catch.hpp>

#include <sec21/access.h>
#include <sec21/file_loader.h>
#include <sec21/numeric/drop.h>
#include <sec21/numeric/flatten.h>
#include <sec21/numeric/ublas_allocator_wrapper.h>
#include <sec21/structural_analysis/loadcase.h>
#include <sec21/structural_analysis/solve.h>
#include <sec21/structural_analysis/node.h>
#include <sec21/structural_analysis/member.h>
#include <sec21/structural_analysis/space_truss.h>
#include <sec21/structural_analysis/system_result.h>
#include <sec21/units.h>

#include <sec21/strong_type.h>
#include <sec21/policy/compare.h>
#include <sec21/policy/print.h>
#include <sec21/policy/increment_alphabet.h>
#include <sec21/policy/compare_less.h>

#include <nlohmann/json.hpp>

#include <string>

namespace v2 {

      template <typename T>
      struct alphabet_compare_less
      {
         friend constexpr bool operator < (T const& , const T& ) noexcept
         {
            // using type = typename T::underlying_t;
            // return static_cast<type const&>(lhs) < static_cast<type const&>(rhs);
            return true;
         }
      };  

   using alphabet_t = sec21::strong_type<
      std::string, 
      struct alphabet_tag, 
      // sec21::policy::compare_less,
      alphabet_compare_less,
      sec21::policy::increment_alphabet, 
      sec21::policy::compare,
      sec21::policy::print>;

   void to_json(nlohmann::json &j, alphabet_t const &obj)
   {
      j = nlohmann::json{ { "value", static_cast<alphabet_t::underlying_t const&>(obj) } };
   }
}


#include <algorithm>
#include <array>
#include <utility>
#include <valarray>
#include <vector>

TEST_CASE("example system 8.0", "[sec21][structural_analysis][space_truss]")
{
   using namespace sec21;
   using namespace v2;
   using namespace sec21::structural_analysis;
   using namespace sec21::units::literals;

   using member_t = member<alphabet_t, double>;
   using node_t = node<2, alphabet_t, double>;
   using space_truss_t = space_truss<node_t, member_t>;

   auto sys = space_truss_t{};

   static_assert(is_space_truss<decltype(sys)>::value);
   static_assert(is_space_truss_2D<decltype(sys)>::value);

   using precision_t = decltype(sys)::precision_t;
   using node_t = decltype(sys)::node_t;
   using support_t = node_t::global_support_t;
   using loadcase_t = loadcase<decltype(sys)>;

   auto n1 = add_node(sys, {alphabet_t{"N1"}, {0.0, 3.0}});
   auto n2 = add_node(sys, {alphabet_t{"N2"}, {3.0, 3.0}});
   auto n3 = add_node(sys, {alphabet_t{"N3"}, {3.0, 0.0}, support_t{false, true}});
   auto n4 = add_node(sys, {alphabet_t{"N4"}, {0.0, 0.0}, support_t{true, true}});

   REQUIRE(static_cast<bool>(n1) == true);
   REQUIRE(static_cast<bool>(n2) == true);
   REQUIRE(static_cast<bool>(n3) == true);
   REQUIRE(static_cast<bool>(n4) == true);

   // //! \todo use units
   // // constexpr auto E = 210.0_kPa;
   // // constexpr auto A = unit::square_meter<double>(0.004);
   auto m1 = add_member(sys, alphabet_t{"N1"}, alphabet_t{"N2"}, {alphabet_t{"A"}, 0.004, 21'000'000});
   auto m2 = add_member(sys, alphabet_t{"N3"}, alphabet_t{"N2"}, {alphabet_t{"B"}, 0.004, 21'000'000});
   auto m3 = add_member(sys, alphabet_t{"N4"}, alphabet_t{"N3"}, {alphabet_t{"C"}, 0.004, 21'000'000});
   auto m4 = add_member(sys, alphabet_t{"N4"}, alphabet_t{"N1"}, {alphabet_t{"D"}, 0.004, 21'000'000});
   auto m5 = add_member(sys, alphabet_t{"N4"}, alphabet_t{"N2"}, {alphabet_t{"E"}, 0.004, 21'000'000});
   auto m6 = add_member(sys, alphabet_t{"N3"}, alphabet_t{"N1"}, {alphabet_t{"F"}, 0.004, 21'000'000});

   REQUIRE(static_cast<bool>(m1) == true);
   REQUIRE(static_cast<bool>(m2) == true);
   REQUIRE(static_cast<bool>(m3) == true);
   REQUIRE(static_cast<bool>(m4) == true);
   REQUIRE(static_cast<bool>(m5) == true);
   REQUIRE(static_cast<bool>(m6) == true);

   {
      std::ofstream ofs{"output_example_8.json"};
      nlohmann::json j = sys;
      ofs << std::setw(4) << j;
   }
}