#pragma once

#include <boost/qvm/vec.hpp>

#include <sec21/units.h>
#include <sec21/structural_analysis/impl/geometry.h>

#include <array>
#include <cmath>

namespace sec21::structural_analysis::impl
{
   //! \todo 2019-04-09 return value unit::kilonetwon<System::precision>
   //! \todo 2019-09-10 parameter verschiebungen sollte vom Type ein std::array<UNIT, 4> sein
   template <typename System>
   [[nodiscard]] auto normal_force(
      System const& sys, 
      typename System::member_descriptor_t id,
      std::array<double, 4> const& verschiebungen) -> typename System::precision_t // aus spannungsmatrix
   {
      static_assert(System::node_t::dimension_v == 2, "Only works with 2D system");

      const auto alpha = angle_to_x_axis(sys, id);
      const auto k1 = alpha < 0.0 ? -1.0 : 1.0;
      // clang-format off
      return
         (-std::cos(alpha) * std::get<0>(verschiebungen) +
          -std::sin(alpha) * std::get<1>(verschiebungen) +
           std::cos(alpha) * std::get<2>(verschiebungen) +
           std::sin(alpha) * std::get<3>(verschiebungen)) * k1;
      // clang-format on
   }

   template <typename System>
#ifdef __cpp_concepts
   requires SpaceTruss<System>
#endif   
   [[nodiscard]] auto normal_force(
      System const& sys, 
      typename System::member_descriptor_t const& id, 
      boost::qvm::vec<units::quantity<units::millimeter, 2> const& displacement_from_node,
      boost::qvm::vec<units::quantity<units::millimeter, 2> const& displacement_to_node)

      -> units::quantity<units::newton, typename System::precision_t>
   {
      // berrechnet aus den knotenverschiebungen und dem neigungswinkel des stabes die normalkraft im stab


      return {};
   }

}