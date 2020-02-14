#pragma once

#include <sec21/units.h>
#include <sec21/strong_type.h>
#include <sec21/structural_analysis/support.h>
#include <sec21/structural_analysis/force.h>

#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/core/cs.hpp>

#include <type_traits>
#include <limits>

namespace sec21::structural_analysis
{
   template <auto Dimension, typename Precision = double> //, template<typename> typename... Policies>
   struct node // : public Policies<system<Dimension, Policies...>>...   
   {
      static_assert(Dimension == 2, "Only works in 2D just yet");
      static_assert(std::is_floating_point<Precision>::value, "Precision is not a floating-point type");
      static_assert(std::numeric_limits<Precision>::is_iec559, "Floating-point type does not fulfill the requirements of IEC 559");

      constexpr static auto dimension_v = Dimension;

      using descriptor_t = size_t;
      using precision_t = Precision;
      using point_t = boost::geometry::model::point<precision_t, dimension_v, boost::geometry::cs::cartesian>;
      using load_t = force_t<dimension_v>;

      //! \todo 2019-04-23 use strong_type
      descriptor_t id{ std::numeric_limits<descriptor_t>::max() };
      //! \brief World Position
      point_t        position{};
      //! \brief
      Support        support{ Support::Free };
      //! \brief
      load_t    load{};
   };

   template <typename T>
   struct is_node : std::false_type {};

   template <auto Dimension, typename Precision>
   struct is_node<node<Dimension, Precision>> : std::true_type {};

   template <typename T>
   constexpr bool is_node_v = is_node<T>::value;

   static_assert(is_node_v<node<2, float>>, "should be a true");
   static_assert(is_node_v<node<2, double>>, "should be a true");
   static_assert(is_node_v<int> == false, "should be a false");

#ifdef __cpp_concepts
   template <typename T>
   concept Node = is_node<T>::value;
#endif
}
