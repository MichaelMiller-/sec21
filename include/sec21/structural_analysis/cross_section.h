#pragma once

#include <sec21/structural_analysis/descriptor_traits.h>
#include <sec21/units.h>

#include <boost/uuid/uuid.hpp>

namespace sec21::structural_analysis
{
   struct cross_section
   {
      using descriptor_t = std::size_t;
      using precision_t = float;
      using area_t = units::quantity<units::square_meter, precision_t>;

      static_assert(std::is_floating_point_v<precision_t>);

      descriptor_t id{descriptor_traits<descriptor_t>::invalid()};

      std::string name{};

      boost::uuids::uuid tag{};

      //! \brief DE: Querschnittsfläche
      area_t A;

      std::string material_name{};
   };
} // namespace sec21::structural_analysis
