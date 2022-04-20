#pragma once

#include <sec21/structural_analysis/descriptor_traits.h>

#include <boost/algorithm/string/predicate.hpp>
#include <boost/uuid/uuid.hpp>

#include <string>

namespace sec21::structural_analysis
{
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

   enum class RotationalStiffness {
      Free,
      Rigid,
      Flexible,
      NonLinear,
   };

   //! \todo replace with c++ reflect -> enum_to_string.h
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

   inline constexpr auto to_string(RotationalStiffness value)
   {
      if (value == RotationalStiffness::Free)
         return "Free";
      if (value == RotationalStiffness::Rigid)
         return "Rigid";
      if (value == RotationalStiffness::Flexible)
         return "Flexible";
      if (value == RotationalStiffness::NonLinear)
         return "NonLinear";
   }

   template <typename T>
   constexpr auto to_enum(std::string_view);

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
      static_assert(size(names) == 8);

      const auto it =
         std::find_if(begin(names), end(names), [value](auto const& e) { return boost::iequals(e.first, value); });
      if (it == end(names))
         throw std::runtime_error("cannot convert string to enum");
      return it->second;
   }

   template <>
   constexpr auto to_enum<RotationalStiffness>(std::string_view value)
   {
      constexpr auto names = std::array{
         std::pair{"Free", RotationalStiffness::Free},
         std::pair{"Rigid", RotationalStiffness::Rigid},
         std::pair{"Flexible", RotationalStiffness::Flexible},
         std::pair{"NonLinear", RotationalStiffness::NonLinear},
      };
      static_assert(size(names) == 4);

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
      using precision_t = double;

      //! \brief can be empty
      std::string name{};

      boost::uuids::uuid tag{};

      //! \brief Translation in X direction
      Translation ux{Translation::Free};
      //! \brief Translation in Y direction
      Translation uy{Translation::Free};
      //! \brief Translation in Z direction
      Translation uz{Translation::Free};
      //! \brief Rotational stiffness around X axis
      RotationalStiffness fix{RotationalStiffness::Free};
      //! \brief Rotational stiffness around Y axis
      RotationalStiffness fiy{RotationalStiffness::Free};
      //! \brief Rotational stiffness around Z axis
      RotationalStiffness fiz{RotationalStiffness::Free};

      //! \brief The flexibility of the support in X direction
      precision_t stiffness_x{}; // [MN/m]
      //! \brief The flexibility of the support in Y direction
      precision_t stiffness_y{}; // [MN/m]
      //! \brief The flexibility of the support in Z direction
      precision_t stiffness_z{}; // [MN/m]

      //! \brief The flexibility in rotation of the connection around local X axis
      precision_t stiffness_fix{}; // [MNm/rad]
      //! \brief The flexibility in rotation of the connection around local Y axis
      precision_t stiffness_fiy{}; // [MNm/rad]
      //! \brief The flexibility in rotation of the connection around local Z axis
      precision_t stiffness_fiz{}; // [MNm/rad]
   };

   inline constexpr auto has_spring(point_support const& value) noexcept
   {
      return value.ux == Translation::Flexible || value.uy == Translation::Flexible ||
             value.uz == Translation::Flexible || value.fix == RotationalStiffness::Flexible ||
             value.fiy == RotationalStiffness::Flexible || value.fiz == RotationalStiffness::Flexible;
   };
} // namespace sec21::structural_analysis
