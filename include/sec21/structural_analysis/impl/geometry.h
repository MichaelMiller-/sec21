#pragma once

#include <sec21/access.h>

#ifdef __cpp_lib_math_constants
   #include <numbers>
   #warning TODO: replace with std::numbers::
#else 
   #include <boost/math/constants/constants.hpp>
#endif

#include <boost/geometry/geometries/point.hpp>
#include <boost/qvm/all.hpp>
#include <boost/qvm/quat_traits_array.hpp>

namespace sec21::customization_point
{
   template <typename T, auto N, typename CoordinateSystem>
   struct access_x<boost::geometry::model::point<T, N, CoordinateSystem>>
   {
      auto operator()(boost::geometry::model::point<T, N, CoordinateSystem> const& t) const noexcept
      {
         return boost::geometry::get<0>(t);
      }
      auto& operator()(boost::geometry::model::point<T, N, CoordinateSystem>&& t) const noexcept
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
      auto& operator()(boost::geometry::model::point<T, N, CoordinateSystem>&& t) const noexcept
      {
         return boost::geometry::get<1>(t);
      }        
   };   
}

#if 0 //! \todo not working
template <typename T, auto N, typename CoordinateSystem>
using point_t = boost::geometry::model::point<T, N, CoordinateSystem>;

namespace boost::qvm 
{
   template <typename T, auto N, typename CoordinateSystem>
   struct vec_traits<point_t<T, N, CoordinateSystem>> : vec_traits_defaults<point_t<T, N, CoordinateSystem>, T, N> 
   {
      using scalar_type = T;

      template <int I>
      static inline scalar_type& write_element(point_t<T, N, CoordinateSystem>& v) 
      {
         return boost::geometry::get<I>(v);
         // static double r{};
         // return r;
      }

      // static inline scalar_type & write_element_idx(int i, point_t& v) 
      // {
      //    return v.a[i];
      // } //optional
   };
}
#endif

namespace boost::qvm 
{
   template <typename T, auto N>
   struct vec_traits<std::array<T, N>> : vec_traits_defaults<std::array<T, N>, T, N> 
   {
      using scalar_type = T;

      template <int I>
      static inline scalar_type& write_element(std::array<T, N>& v) 
      {
         return std::get<I>(v);
      }
   };
}

namespace sec21::structural_analysis::impl
{
   //! \todo  angle_to(...., Axis == )
   // template <typename Rotation> // = CounterClockwise>
   // struct angle_t
   // {
   //    auto degree() const { return 3; }
   // };

   template <typename T>
   auto angle_to_x_axis(T const& start_position, T const& end_position) noexcept -> double
   {
      using value_t = std::array<double, 2>;
      //! \todo remove conversion
      auto from = value_t{ X(start_position), Y(start_position) };
      auto to = value_t{ X(end_position), Y(end_position) };

      using namespace boost::qvm;

      const auto dir = to - from;
      decltype(dir) project_x{ X(dir) };
      const auto m = dot(dir, project_x);
      const auto l1 = mag(dir);
      const auto l2 = mag(project_x);

      //! \todo approx
      if (m == 1)
         return boost::math::constants::half_pi<double>() * 0.5;

      if (0 == (l1 * l2))
         return boost::math::constants::half_pi<double>();

      auto k1 = 1;
      if (Y(dir) < 0)
         k1 *= -1;
      if (X(dir) < 0)
         k1 *= -1;

      //! \todo odd accos function?
      return std::acos(m / (l1 * l2)) * k1;
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