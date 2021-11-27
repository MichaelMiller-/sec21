#pragma once

#include <sec21/structural_analysis/descriptor_traits.h>

#include <boost/uuid/uuid.hpp>

#include <string>

namespace sec21::structural_analysis
{
   struct point_action
   {
      using descriptor_t = std::size_t;
      using precision_t = float;
      using value_t = units::quantity<units::newton, precision_t>;
      static_assert(std::is_floating_point_v<precision_t>);

      descriptor_t id{ descriptor_traits<descriptor_t>::invalid() };

      //! \brief can be empty
      std::string name{};

      boost::uuids::uuid tag{};

      std::string structural_point_name{};
      std::string load_case_name{};

      //! \todo replace with value_t
      precision_t delta_x{};
      precision_t delta_y{};
      precision_t delta_z{};
   };
} // namespace sec21::structural_analysis
