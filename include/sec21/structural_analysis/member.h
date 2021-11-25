#pragma once

#include <sec21/structural_analysis/concepts.h>
#include <sec21/structural_analysis/descriptor_traits.h>
#include <sec21/units.h>

#include <boost/uuid/uuid.hpp>

//! \todo 2019-05-10 local coordinate system
namespace sec21::structural_analysis
{
   template <typename Descriptor, typename Precision = double>
   struct member
   {
      using descriptor_t = Descriptor;
      using precision_t = Precision;
      using e_modul_t = units::quantity<units::kilopascal, precision_t>;
      using area_t = units::quantity<units::square_meter, precision_t>;

      descriptor_t id{descriptor_traits<descriptor_t>::invalid()};

      std::string name{};

      boost::uuids::uuid tag{};

      std::string material_name{};
      std::string cross_section_name{};

      //! \todo querschnittswerte -> new class: "cross_section"; ref to cross_section;
      //! \brief
      e_modul_t E;
      //! \brief
      area_t A;
   };
} // namespace sec21::structural_analysis

#include <nlohmann/json.hpp>

namespace sec21::structural_analysis
{
   template <typename Descriptor, typename Precision>
   void to_json(nlohmann::json& j, member<Descriptor, Precision> const& obj)
   {
      j = nlohmann::json{{"name", obj.id},
                         //! \todo {"name", obj.name},
                         {"E", obj.E},
                         {"A", obj.A}};
   }

   template <typename Descriptor, typename Precision>
   void from_json(nlohmann::json const& j, member<Descriptor, Precision>& obj)
   {
      j.at("name").get_to(obj.id);
      j.at("E").get_to(obj.E);
      j.at("A").get_to(obj.A);
   }
} // namespace sec21::structural_analysis
