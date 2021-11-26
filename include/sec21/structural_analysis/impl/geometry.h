#pragma once

#include <sec21/array_qvm_vec_traits.h>
#include <sec21/structural_analysis/concepts.h>
#include <sec21/structural_analysis/node.h>
#include <sec21/units/dimensions/length.h>

#ifdef __cpp_lib_math_constants
#include <numbers>
#else
#include <boost/math/constants/constants.hpp>
#endif

#include <boost/qvm/vec.hpp>
#include <boost/qvm/vec_access.hpp>
#include <boost/qvm/vec_operations.hpp>

namespace sec21::structural_analysis::impl
{
   template <typename T>
   [[nodiscard]] auto distance(T const& from, T const& to)
   {
      using namespace boost::qvm;
      return mag(to - from);
   }

   template <auto Dimension, typename Precision>
   [[nodiscard]] auto distance(node<Dimension, Precision> const& from, node<Dimension, Precision> const& to)
   {
      using namespace boost::qvm;
      return mag(to.position - from.position);
   }

   template <typename System>
   [[nodiscard]] auto length(System const& sys, typename System::member_descriptor_t id)
      -> units::quantity<units::meter, typename System::precision_t>
   {
      const auto [s, e] = sys.coincidence_table.at(id);

      // to prevent bug in clang use generalized lambda capture
      const auto from = std::find_if(begin(sys.nodes), end(sys.nodes), [s = s](auto const& n) { return n.id == s; });
      const auto to = std::find_if(begin(sys.nodes), end(sys.nodes), [e = e](auto const& n) { return n.id == e; });

      if (from == end(sys.nodes) || to == end(sys.nodes))
         throw std::invalid_argument("invalid node reference");

      return {distance(*from, *to)};
   }

   //! \todo  angle_to(...., Axis == )
   // template <typename Rotation> // = CounterClockwise>
   // struct angle_t
   // {
   //    auto degree() const { return 3; }
   // };

   // template <typename T>
   // auto close_to(T lhs, T rhs, T epsilon = std::numeric_limits<T>::epsilon())
   // {
   //   return std::fabs(lhs - rhs) < epsilon;
   // }

   template <typename T>
   [[nodiscard]] auto angle_to_x_axis(T const& from, T const& to) noexcept
   {
      using namespace boost::qvm;
      using value_t = typename vec_traits<T>::scalar_type;
      constexpr auto dim_v = vec_traits<T>::dim;

      const auto dir = to - from;
      const auto project_x = vec<value_t, dim_v>{X(dir)};
      const auto m = dot(dir, project_x);
      const auto l1 = mag(dir);
      const auto l2 = mag(project_x);

#ifdef __cpp_lib_math_constants
      constexpr auto half_pi = std::numbers::pi_v<value_t> * .5;
#else
      constexpr auto half_pi = boost::math::constants::half_pi<value_t>();
#endif
      //! \todo approx
      if (m == 1)
         return half_pi * 0.5;

      if (0 == (l1 * l2))
         return half_pi;

      auto k1 = 1;

      if (Y(dir) < 0)
         k1 *= -1;
      if (X(dir) < 0)
         k1 *= -1;

      return std::acos(m / (l1 * l2)) * k1;
   }

   template <auto Dimension, typename Precision>
   [[nodiscard]] auto angle_to_x_axis(node<Dimension, Precision> const& from, node<Dimension, Precision> const& to)
   {
      return angle_to_x_axis(from.position, to.position);
   }

   template <typename System>
   [[nodiscard]] auto angle_to_x_axis(System const& sys, typename System::member_descriptor_t id)
   {
      const auto [s, e] = sys.coincidence_table.at(id);

      // to prevent bug in clang use generalized lambda capture
      const auto from = std::find_if(begin(sys.nodes), end(sys.nodes), [s = s](auto const& n) { return n.id == s; });
      const auto to = std::find_if(begin(sys.nodes), end(sys.nodes), [e = e](auto const& n) { return n.id == e; });

      if (from == end(sys.nodes) || to == end(sys.nodes))
         throw std::invalid_argument("invalid node reference");

      return angle_to_x_axis(*from, *to);
   };
} // namespace sec21::structural_analysis::impl