#pragma once

#include <boost/math/constants/constants.hpp>

#include <sec21/access.h>
#include <sec21/structural_analysis/type_traits.h>
#include <sec21/structural_analysis/force.h>
#include <sec21/units/dimensions/force.h>
#include <sec21/units/quantity.h>

#include <vector>

namespace sec21::structural_analysis::impl
{
   //! \brief Knotenpunktslasten F
   template <typename System>
   [[nodiscard]] auto load_vector(System const& sys) noexcept 
      -> std::enable_if_t<is_space_truss_2D<System>::value, std::vector<units::quantity<units::newton, typename System::precision_t>>>
   {
      using node_t = typename decltype(sys.nodes)::value_type;
      using load_t = typename node_t::load_t;
      using value_t = units::quantity<units::newton, typename System::precision_t>;
     
      std::vector<value_t> result(std::size(sys.nodes) * System::dimension_v);
      auto it = std::begin(result);

      //! \todo std::copy 
      for (auto n : sys.nodes)
      {
         if (n.load.has_value())
         {
            *it = X(n.load.value()); ++it;
            *it = Y(n.load.value()); ++it;
         }
         else
            std::advance(it, System::dimension_v);
      }
      return result;
   }
}