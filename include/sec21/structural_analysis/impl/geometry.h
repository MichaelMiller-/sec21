#pragma once

#include <sec21/structural_analysis/concepts.h>
#include <sec21/structural_analysis/node.h>
// #include <sec21/structural_analysis/error_codes.h>
#include <sec21/units/dimensions/length.h>

#ifdef __cpp_lib_math_constants
   #include <numbers>
   //! \todo use
#else 
   #include <boost/math/constants/constants.hpp>
#endif

#include <boost/qvm/vec_traits_defaults.hpp>
#include <boost/qvm/vec_operations.hpp>
#include <boost/qvm/vec_access.hpp>

#include <stdexcept>

namespace boost::qvm 
{
   template <typename T, auto N>
   struct vec_traits<std::array<T, N>> : vec_traits_defaults<std::array<T, N>, T, N> 
   {
      using scalar_type = T;

      template <auto I>
      static inline scalar_type& write_element(std::array<T, N>& v) 
      {
         return std::get<I>(v);
      }
   };
}

namespace sec21::structural_analysis::impl
{
   template <auto Dimension, typename Precision>
   [[nodiscard]] auto distance(node<Dimension, Precision> const& from, node<Dimension, Precision> const& to) -> Precision
   {
      using namespace boost::qvm;
      return { mag(to.position - from.position) };
   }

   template <typename System>
   [[nodiscard]] auto length(System const& sys, typename System::member_descriptor_t id) -> units::quantity<units::meter, typename System::precision_t>
   {
      const auto [s, e] = sys.coincidence_table.at(id);
      
      const auto from = std::find_if(
         begin(sys.nodes), 
         end(sys.nodes), 
         [s](auto const& n){ return n.id == s; });

      const auto to = std::find_if(
         begin(sys.nodes), 
         end(sys.nodes), 
         [e](auto const& n){ return n.id == e; });

      if (from == end(sys.nodes) || to == end(sys.nodes))
         throw std::invalid_argument("invalid node reference");

      return { distance(*from, *to) };
   }   

   //! \todo 
   // auto start_and_end_position() -> std::pair<>
   // {
   //    const auto[s, e] = sys.coincidence_table.at(id);
   //    const auto from = get_element(sys.nodes, s);
   //    const auto to = get_element(sys.nodes, e);      
   // }

   //! \todo  angle_to(...., Axis == )
   // template <typename Rotation> // = CounterClockwise>
   // struct angle_t
   // {
   //    auto degree() const { return 3; }
   // };

   template <typename Position>
   [[nodiscard]] auto angle_to_x_axis(Position const& from, Position const& to) noexcept -> double
   {
      using namespace boost::qvm;
      using namespace boost::math;

      const auto dir = to - from;
      decltype(dir) project_x{ X(dir) };
      const auto m = dot(dir, project_x);
      const auto l1 = mag(dir);
      const auto l2 = mag(project_x);

      //! \todo approx
      if (m == 1)
         return constants::half_pi<typename vec_traits<Position>::scalar_type>() * 0.5;

      if (0 == (l1 * l2))
         return constants::half_pi<typename vec_traits<Position>::scalar_type>();

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

      const auto from = std::find_if(
         begin(sys.nodes), 
         end(sys.nodes), 
         [s](auto const& n){ return n.id == s; });

      const auto to = std::find_if(
         begin(sys.nodes), 
         end(sys.nodes), 
         [e](auto const& n){ return n.id == e; });

      if (from == end(sys.nodes) || to == end(sys.nodes))
         throw std::invalid_argument("invalid node reference");

      return angle_to_x_axis(*from, *to);
   };
}