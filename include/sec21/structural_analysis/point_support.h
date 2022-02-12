#pragma once

#include <sec21/structural_analysis/descriptor_traits.h>

#include <boost/algorithm/string/predicate.hpp>
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

   inline constexpr auto to_string(ConstraintSupport value)
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
   inline constexpr auto to_string(Translation value)
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

   template <typename T>
   constexpr auto to_enum(std::string_view);

   template <>
   constexpr auto to_enum<ConstraintSupport>(std::string_view value)
   {
      constexpr auto names = std::array{
         std::pair{"Fixed", ConstraintSupport::Fixed},
         std::pair{"Hinged", ConstraintSupport::Hinged},
         std::pair{"Sliding", ConstraintSupport::Sliding},
         std::pair{"Custom", ConstraintSupport::Custom},
      };
      //! \todo magic number! count the number of enum-items
      static_assert(size(names) == 4);

      const auto it =
         std::find_if(begin(names), end(names), [value](auto const& e) { return boost::iequals(e.first, value); });
      if (it == end(names))
         throw std::runtime_error("cannot convert string to enum");
      return it->second;
   }

   template <>
   constexpr auto to_enum<Translation>(std::string_view value)
   {
      constexpr auto names = std::array{
         std::pair{"Rigid", Translation::Rigid},
         std::pair{"Free", Translation::Free},
         std::pair{"Flexible", Translation::Flexible},
         std::pair{"CompressionOnly", Translation::CompressionOnly},
         std::pair{"TensionOnly", Translation::TensionOnly},
         std::pair{"FlexibleCompressionOnly", Translation::FlexibleCompressionOnly},
         std::pair{"FlexibleTensionOnly", Translation::FlexibleTensionOnly},
         std::pair{"NonLinear", Translation::NonLinear},
      };
      //! \todo magic number! count the number of enum-items
      static_assert(size(names) == 8);

      const auto it =
         std::find_if(begin(names), end(names), [value](auto const& e) { return boost::iequals(e.first, value); });
      if (it == end(names))
         throw std::runtime_error("cannot convert string to enum");
      return it->second;
   }

   struct point_support
   {
      using descriptor_t = std::size_t;
      descriptor_t id{descriptor_traits<descriptor_t>::invalid()};

      //! \brief can be empty
      std::string name{};

      boost::uuids::uuid tag{};

      ConstraintSupport type{ConstraintSupport::Fixed};

      Translation ux{Translation::Free};
      Translation uy{Translation::Free};
      Translation uz{Translation::Free};
      Translation fix{Translation::Free};
      Translation fiy{Translation::Free};
      Translation fiz{Translation::Free};
   };
} // namespace sec21::structural_analysis
