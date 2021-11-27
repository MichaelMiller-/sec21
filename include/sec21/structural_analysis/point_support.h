#pragma once

#include <sec21/structural_analysis/descriptor_traits.h>

#include <boost/uuid/uuid.hpp>

#include <string>

namespace sec21::structural_analysis
{
   enum class ConstraintSupport { Fixed, Hinged, Sliding, Custom };

   enum class Translation {
      Rigid,
      Free,
      Flexible,
      CompressionOnly,
      TensionOnly,
      FlexibleCompressionOnly,
      FlexibleTensionOnly,
      NonLinear,
   };

//! \todo there i a better way to do that !!!
   inline auto to_string(ConstraintSupport value) -> std::string
   {
      if (value == ConstraintSupport::Fixed)
         return "Fixed";
      if (value == ConstraintSupport::Hinged)
         return "Hinged";
      if (value == ConstraintSupport::Sliding)
         return "Sliding";
      if (value == ConstraintSupport::Custom)
         return "Custom";
   }
   inline auto to_string(Translation value) -> std::string
   {
      if (value == Translation::Rigid)
         return "Rigid";
      if (value == Translation::Free)
         return "Free";
      if (value == Translation::Flexible)
         return "Flexible";
      if (value == Translation::CompressionOnly)
         return "CompressionOnly";
      if (value == Translation::TensionOnly)
         return "TensionOnly";
      if (value == Translation::FlexibleCompressionOnly)
         return "FlexibleCompressionOnly";
      if (value == Translation::FlexibleTensionOnly)
         return "FlexibleTensionOnly";
      if (value == Translation::NonLinear)
         return "NonLinear";
   }

   struct point_support
   {
      using descriptor_t = std::size_t;
      descriptor_t id{descriptor_traits<descriptor_t>::invalid()};

      //! \brief can be empty
      std::string name{};

      boost::uuids::uuid tag{};

      std::string node_name{};

      ConstraintSupport type{ConstraintSupport::Fixed};

      Translation ux{Translation::Free};
      Translation uy{Translation::Free};
      Translation uz{Translation::Free};
      Translation fix{Translation::Free};
      Translation fiy{Translation::Free};
      Translation fiz{Translation::Free};
   };
} // namespace sec21::structural_analysis
