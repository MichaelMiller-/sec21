#pragma once

#include <sec21/units.h>
#include <sec21/structural_analysis/descriptor_traits.h>
#include <sec21/structural_analysis/concepts.h>

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

      //! \brief unique name
      descriptor_t name{ descriptor_traits<descriptor_t>::invalid() };

      //! \todo querschnittswerte -> new class: "cross_section"; ref to cross_section; 
      //! \brief
      e_modul_t E;
      //! \brief
      area_t A;
   };
}

#include <nlohmann/json.hpp>

namespace sec21::structural_analysis
{
   template <typename Descriptor, typename Precision>
   void to_json(nlohmann::json& j, member<Descriptor, Precision> const& obj) {
      j = nlohmann::json{
         {"name", obj.name},
         {"E", obj.E},
         {"A", obj.A}
      };
   }

   template <typename Descriptor, typename Precision>
   void from_json(nlohmann::json const& j, member<Descriptor, Precision>& obj) 
   {
      j.at("name").get_to(obj.name);
      j.at("E").get_to(obj.E);
      j.at("A").get_to(obj.A);
   }
}