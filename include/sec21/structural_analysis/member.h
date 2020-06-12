#pragma once

#include <sec21/units.h>
#include <sec21/structural_analysis/concepts.h>

//! \todo 2019-04-23 could be removed if id is a strong_type
//! \todo 2019-05-10 intruduce local coordinate system
//! \todo 2020-01-18 descriptor_t should have an overload from invalid_id()
#include <limits>
#include <type_traits>

namespace sec21::structural_analysis
{
   struct member
   {
      using descriptor_t = size_t;
      using precision_t = double;
      using e_modul_t = units::quantity<units::kilopascal, precision_t>;
      using area_t = units::quantity<units::square_meter, precision_t>;

      descriptor_t id{ std::numeric_limits<descriptor_t>::max() };
      e_modul_t E;
      area_t A;
   };
}

#include <nlohmann/json.hpp>

namespace sec21::structural_analysis
{
   void to_json(nlohmann::json& j, member const& obj) {
      j = nlohmann::json{
         {"id", obj.id},
         {"E", obj.E},
         {"A", obj.A}
      };
   }
   void from_json(nlohmann::json const& j, member& obj) 
   {
      j.at("id").get_to(obj.id);
      j.at("E").get_to(obj.E);
      j.at("A").get_to(obj.A);
   }
}