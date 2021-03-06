#pragma once

#include <sec21/numeric/make_matrix.h>
#include <sec21/structural_analysis/common.h>
#include <sec21/structural_analysis/impl/geometry.h>

#include <boost/math/special_functions/sin_pi.hpp>
#include <boost/math/special_functions/cos_pi.hpp>

#include <vector>
#include <algorithm>

namespace sec21::structural_analysis::impl
{
   // Steifigkeits-beziehung/-matrizen des Fachwerkstabs in globalen Koordinaten:
   //
   //         |  c^2   s*c  -c^2  -s*c |   | u_1 |     | Fx1 |
   //   E*A   |  s*c   s^2  -s*c  -s^2 |   | v_1 |     | Fy1 |
   //   --- * | -c^2  -s*c   c^2   s*c | * | u_2 |  =  | Fx2 | 
   //    l    | -s*c  -s^2   s*c   s^2 |   | v_2 |     | Fy2 | 
   //

   //! \todo rename
   template <typename Allocator, typename System>
   [[nodiscard]] auto steifigkeitsbeziehung_fachwerkstab_globalen_koordinaten(
      System const& sys,
      typename System::member_descriptor_t id)
   {
      using precision_t = typename System::precision_t;
      static_assert(std::is_same_v<typename std::allocator_traits<Allocator>::value_type, precision_t>);

      const auto angle = angle_to_x_axis(sys, id);

      auto it = std::find_if(
         std::begin(sys.members), 
         std::end(sys.members), 
         [&id](auto&& m) { return m.name == id; });

      if (it == std::end(sys.members))
         throw std::invalid_argument("invalid member descriptor"); //fmt::format("invalid member descriptor: {}"), id);

      const auto kv = EA_over_l(it->E, it->A, length(sys, it->name)).value();

      using namespace boost::math;
      //! \note this approch (rad / pi) get better results -> so the minimal overhead is tolerable
      const auto npi = angle / constants::pi<decltype(angle)>();
      auto s = sin_pi(npi);
      auto c = cos_pi(npi);

      //! \todo approx
      if (angle == 0.0)
      {
         s = 0;
         c = 1;
      }
      if (angle == constants::half_pi<precision_t>())
      {
         s = 1;
         c = 0;
      }

      // clang-format off
      const auto values = {
         (c * c) * kv,  (s * c) * kv, (-c * c) * kv, (-s * c) * kv,
         (s * c) * kv,  (s * s) * kv, (-s * c) * kv, (-s * s) * kv,
         (-c * c) * kv, (-s * c) * kv,  (c * c) * kv,  (s * c) * kv,
         (-s * c) * kv, (-s * s) * kv,  (s * c) * kv,  (s * s) * kv,
      };
      // clang-format on
      return numeric::make_matrix<Allocator>(4, 4, std::begin(values), std::end(values));
   }
}