#pragma once

#include <sec21/structural_analysis/impl/geometry.h>
#include <sec21/structural_analysis/impl/matrix_helper.h>

namespace sec21::structural_analysis::impl
{
   // Steifigkeits-beziehung/-matrizen des Fachwerkstabs in globalen Koordinaten:
   //
   //         |  c^2   s*c  -c^2  -s*c |   | u_1 |     | Fx1 |
   //   E*A   |  s*c   s^2  -s*c  -s^2 |   | v_1 |     | Fy1 |
   //   --- * | -c^2  -s*c   c^2   s*c | * | u_2 |  =  | Fx2 | 
   //    l    | -s*c  -s^2   s*c   s^2 |   | v_2 |     | Fy2 | 
   //
   template <typename System>
   [[nodiscard]] auto steifigkeitsbeziehung_fachwerkstab_globalen_koordinaten(
      System const& sys,
      typename System::member_descriptor_t id)
   {
      const auto[start_node, end_node] = sys.adjacency_list.at(id);
      const auto angle = angle_to_x_axis(sys.nodes.at(start_node), sys.nodes.at(end_node));

      using namespace boost::math;

      //! \todo lookup for known angle
      //! \todo approx to zero
      if (angle == 0.0)
      {
         // clang-format off
         const auto values = std::array<double, 4*4>{
            1, 0,-1, 0,
            0, 0, 0, 0,
            -1, 0, 1, 0,
            0, 0, 0, 0
         };
         // clang-format on
         return make_matrix(4, 4, std::begin(values), std::end(values));
      }
      if (angle == constants::half_pi<typename System::precision_t>())
      {
         // clang-format off
         const auto values = std::array<double, 4*4>{
            0, 0, 0, 0,
            0, 1, 0,-1,
            0, 0, 0, 0,
            0,-1, 0, 1
         };
         // clang-format on
         return make_matrix(4, 4, std::begin(values), std::end(values));
      }

      //! \note this approch (rad / pi) get better results -> so the minimal overhead is tolerable
      const auto npi = angle / constants::pi<decltype(angle)>();
      const auto s{ sin_pi(npi) };
      const auto c{ cos_pi(npi) } ;
      //! \todo durch x-y-anteil des stabes teilen?
      const auto k{ std::sin(std::abs(angle)) + std::cos(std::abs(angle)) };

      // clang-format off
      const auto values = std::array{
         (c * c) / k,  (s * c) / k, (-c * c) / k, (-s * c) / k,
         (s * c) / k,  (s * s) / k, (-s * c) / k, (-s * s) / k,
         (-c * c) / k, (-s * c) / k,  (c * c) / k,  (s * c) / k,
         (-s * c) / k, (-s * s) / k,  (s * c) / k,  (s * s) / k,
      };
      // clang-format on
      return make_matrix(4, 4, std::begin(values), std::end(values));
   }
}