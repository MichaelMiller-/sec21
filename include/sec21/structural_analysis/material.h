#pragma once

#include <sec21/structural_analysis/descriptor_traits.h>

#include <boost/uuid/uuid.hpp>

#include <string>

namespace sec21::structural_analysis
{
   struct material
   {
      using descriptor_t = std::size_t;
      using precision_t = double;

      static_assert(std::is_floating_point_v<precision_t>);
      static_assert(is_descriptor<descriptor_t>::value);

      descriptor_t id{ descriptor_traits<descriptor_t>::invalid() };

      std::string name{};

      boost::uuids::uuid tag{};

      //! \brief Young_modulus \[ E = \frac{\sigma}{\epsilon} \] [kPa]
      precision_t young_modulus{};
      // using e_modul_t = units::quantity<units::kilopascal, precision_t>;

      precision_t G;

      precision_t alphaT{0.00012}; //!\todo replace with zero
   };
} // namespace sec21::structural_analysis
