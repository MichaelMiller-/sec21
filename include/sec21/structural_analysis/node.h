#pragma once

#include <sec21/units.h>
#include <sec21/strong_type.h>
#include <sec21/structural_analysis/support.h>
#include <sec21/structural_analysis/force.h>

#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/core/cs.hpp>

namespace sec21::structural_analysis
{
   template <size_t Dimension> //, template<typename> typename... Policies>
   struct node // : public Policies<system<Dimension, Policies...>>...   
   {
      static_assert(Dimension == 2, "Only works in 2D just yet");

      constexpr static auto dimension_v = Dimension;

      using descriptor_t = size_t;
      using precision_t = double;
      using point_t = boost::geometry::model::point<precision_t, dimension_v, boost::geometry::cs::cartesian>;
      using force_t = force<dimension_v, unit::kilonewton<precision_t>>;

      //! \todo 2019-04-23 use strong_type
      descriptor_t id{ std::numeric_limits<descriptor_t>::max() };
      //! \brief World Position
      point_t        position{};
      //! \brief
      support_kind   support{ support_kind::Free };
      //! \brief
      force_t        load{};
   };

   static_assert(std::is_default_constructible_v<node<2>>, "node: must be default constructible");
   static_assert(std::is_trivially_copy_constructible_v<node<2>>, "node: must be trivially copy constructible");
}
