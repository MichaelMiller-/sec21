#pragma once

#include <sec21/access.h>

#ifdef __cpp_lib_math_constants
   #include <numbers>
   #warning TODO: replace with std::numbers::
#else 
   #include <boost/math/constants/constants.hpp>
#endif

namespace sec21::customization_point
{
   template <typename T, auto N, typename CoordinateSystem>
   struct access_x<boost::geometry::model::point<T, N, CoordinateSystem>>
   {
      auto operator()(boost::geometry::model::point<T, N, CoordinateSystem> const& t) const noexcept
      {
         return boost::geometry::get<0>(t);
      }
      auto& operator()(boost::geometry::model::point<T, N, CoordinateSystem>& t) const noexcept
      {
         return boost::geometry::get<0>(t);
      }        
   };

   template <typename T, auto N, typename CoordinateSystem>
   struct access_y<boost::geometry::model::point<T, N, CoordinateSystem>>
   {
      auto operator()(boost::geometry::model::point<T, N, CoordinateSystem> const& t) const noexcept
      {
         return boost::geometry::get<1>(t);
      }
      auto& operator()(boost::geometry::model::point<T, N, CoordinateSystem>& t) const noexcept
      {
         return boost::geometry::get<1>(t);
      }        
   };   
}

namespace sec21::structural_analysis::impl
{
   //! \todo  angle_to(...., Axis == )

   template <typename T>
   auto angle_to_x_axis(T const& start_position, T const& end_position) noexcept -> double
   {
      const auto dx = X(end_position) - X(start_position);
      const auto dy = Y(end_position) - Y(start_position);
      const auto m = dy / dx; // steigung der geraden

      if (std::abs(m) == 1)
         return boost::math::constants::half_pi<double>() * 0.5 * m;
      return std::atan(m / 1 + m);
   }

   //! \todo 2019-04-17 error handling -> outcome
   template <typename System>
   auto angle_to_x_axis(System const& sys, typename System::member_descriptor_t id) // -> outcome::std_result<double>
   {
      //! \todo can throw
      const auto[s, e] = sys.coincidence_table.at(id);
      const auto from = get_element(sys.nodes, s);
      const auto to = get_element(sys.nodes, e);

      return angle_to_x_axis(from->position, to->position);
   };
}