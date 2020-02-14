#pragma once

#include <boost/math/constants/constants.hpp>

namespace sec21::structural_analysis::impl
{
   //! \todo 2019-04-17 error handling -> outcome
   template <typename T>
// #ifdef __cpp_concepts
//    requires Node<T>
// #endif      
   auto angle_to_x_axis(T const& start, T const& end) noexcept
   {
      auto pos_start = start.position;
      auto pos_end = end.position;

      //! \todo customzation_point for boost::geometry::point
      namespace bg = boost::geometry;

      auto dx = (bg::get<0>(pos_end) - bg::get<0>(pos_start));
      auto dy = (bg::get<1>(pos_end) - bg::get<1>(pos_start));
      auto m = dy / dx; // steigung der geraden

      if (std::abs(m) == 1)
         return boost::math::constants::half_pi<typename T::precision_t>() * 0.5 * m;
      return std::atan(m / 1 + m);
   }

   //! \todo 2019-04-18 seams that only the unittests need this function
   //! \todo 2019-04-17 error handling -> outcome
   template <typename System>
   auto angle_to_x_axis(System const& sys, typename System::member_descriptor_t id)
   {
      //! \todo can throw
      const auto[s, e] = sys.adjacency_list.at(id);
      return angle_to_x_axis(sys.nodes.at(s), sys.nodes.at(e));
   };
}