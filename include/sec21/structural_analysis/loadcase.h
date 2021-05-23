#pragma once

#include <sec21/units.h>
#include <sec21/structural_analysis/concepts.h>
#include <sec21/structural_analysis/force.h>
#include <sec21/structural_analysis/impl/geometry.h>

#include <string>
#include <vector>
#include <algorithm>

namespace sec21::structural_analysis
{
   // template <SpaceTruss System>
   template <typename System>
   struct temperature_load
   {
      using member_descriptor_t = typename System::member_descriptor_t;
      using precision_t = typename System::precision_t;
      using temperature_t = units::quantity<units::kelvin, precision_t>;

      member_descriptor_t member_id;
      temperature_t delta_t;
      //! \todo use unit 1/K
      precision_t alpha_t;
   };

   //! \todo template <SpaceTruss2D System>
   template <typename System>
   struct loadcase
   {
      static constexpr auto dimension_v = System::dimension_v;

      using precision_t = typename System::precision_t;
      using node_t = typename System::node_t;
      using node_descriptor_t = typename node_t::descriptor_t;
      using load_t = force_t<dimension_v>;
      using temperature_load_t = temperature_load<System>;

      std::string name{};
      //! \todo could be a std::variant
      //! \todo decide pair<id, load> or id inside load
      std::vector<std::pair<node_descriptor_t, load_t>> node_load{};
      std::vector<temperature_load_t> temperature_loads{};
   };

   //! \todo remove template parameter LoadPair
   template <typename System, typename LoadPair, typename Iterator>
   auto copy_load(System const &sys, LoadPair const &load, Iterator first)
   {
      constexpr auto dim = System::dimension_v;
      const auto it = std::find_if(
          begin(sys.nodes),
          end(sys.nodes),
          [&load](auto const &m) { return m.name == load.first; });

      if (it == end(sys.nodes))
         throw std::invalid_argument("invalid node id");

      const auto pos = std::distance(begin(sys.nodes), it) * dim;

      std::advance(first, pos);
      std::copy(begin(load.second), end(load.second), first);
   }

   template <typename System, typename Unit, typename Allocator>
   auto add_node_load(System const &sys, loadcase<System> const &load, std::vector<Unit, Allocator> &F)
   {
      // constexpr auto dim = System::dimension_v;
      //! \todo std::for_each(std::execution::par
      for (auto const &lf : load.node_load)
         copy_load(sys, lf, std::begin(F));
   }

   template <typename System, typename Iterator>
   auto add_temperature_load(System const &sys, loadcase<System> const &load, Iterator first)
   {
      constexpr auto dim = System::dimension_v;
      //! \todo std::for_each(std::execution::par
      for (auto const &lf : load.temperature_loads)
      {
         const auto it = std::find_if(
             begin(sys.members),
             end(sys.members),
             [id = lf.member_id](auto const &m) { return m.name == id; });

         if (it == std::end(sys.members))
            throw std::invalid_argument("member id not found");

         const auto [s, e] = sys.coincidence_table.at(lf.member_id);

         const auto from = std::find_if(
             begin(sys.nodes),
             end(sys.nodes),
             [s](auto const &n) { return n.name == s; });

         const auto to = std::find_if(
             begin(sys.nodes),
             end(sys.nodes),
             [e](auto const &n) { return n.name == e; });

         //! \todo error handling
         const auto alpha = impl::angle_to_x_axis(*from, *to);

         const auto d1 = std::distance(begin(sys.nodes), from) * dim;
         const auto d2 = std::distance(begin(sys.nodes), to) * dim;

         const auto equivalent_force_unit = it->E * it->A * lf.delta_t * lf.alpha_t;
         const auto equivalent_force = equivalent_force_unit.value();

         auto first_node = first;
         auto second_node = first;

         //! \todo use unit system
         //! \todo use operator +=
         std::advance(first_node, d1);
         *first_node++ = std::cos(alpha) * -equivalent_force;
         *first_node = std::sin(alpha) * -equivalent_force;
         std::advance(second_node, d2);
         *second_node++ = std::cos(alpha) * equivalent_force;
         *second_node = std::sin(alpha) * equivalent_force;
      }
   }

   enum class LoadType
   {
      SelfWeight,
      Others,
      Prestress,
      Dynamic,
      Static,
      Temperature,
      Wind,
      Snow,
      Maintenance,
      Fire,
      Moving,
      Seismic,
      Standard
   };

   enum class LoadDuration
   {
      Long,
      Medium,
      Short,
      Instantaneous
   };

   enum class CoordinateSystem
   {
      Global,
      Local
   };

   enum class Direction
   {
      X = 0,
      Y = 1,
      Z = 2
   };

   enum class ActionType
   {
      Permanent,
      Variable,
      Accidental
   };

   struct point_action
   {
      std::string name;
      boost::uuids::uuid tag;
      LoadType type{ LoadType::Standard };
      ActionType action_type{ ActionType::Permanent };
      Direction direction{ Direction::Z };
      // reference_node
      double value; // [kN]
      // loadcase
      CoordinateSystem coordinate_system{ CoordinateSystem::Global };
   };
}

#include <nlohmann/json.hpp>

namespace sec21::structural_analysis
{
   NLOHMANN_JSON_SERIALIZE_ENUM(ActionType, {
      {Permanent, "Permanent"},
      {Variable, "Variable"},
      {Accidental, "Accidental"},
   })

   NLOHMANN_JSON_SERIALIZE_ENUM(LoadType, {
      {SelfWeight, "SelfWeight"},
      {Others, "Others"},
      {Prestress, "Prestress"},
      {Dynamic, "Dynamic"},
      {Static, "Static"},
      {Temperature, "Temperature"},
      {Wind, "Wind"},
      {Snow, "Snow"},
      {Maintenance, "Maintenance"},
      {Fire, "Fire"},
      {Moving, "Moving"},
      {Dynamic, "Dynamic"},
      {Seismic, "Seismic"},
      {Standard, "Standard"},
   })

   NLOHMANN_JSON_SERIALIZE_ENUM(LoadDuration, {
      {Long, "Long"},
      {Medium, "Medium"},
      {Short, "Short"},
      {Instantaneous, "Instantaneous"},
   })

   NLOHMANN_JSON_SERIALIZE_ENUM(CoordinateSystem, {
      {Global, "Global"},
      {Local, "Local"},
   })

   template <typename System>
   void to_json(nlohmann::json &j, temperature_load<System> const &obj)
   {
      j = nlohmann::json{
          {"member_id", obj.member_id},
          {"delta_t", obj.delta_t},
          {"alpha_t", obj.alpha_t}};
   }

   template <typename System>
   void from_json(nlohmann::json const &j, temperature_load<System> &obj)
   {
      j.at("member_id").get_to(obj.member_id);
      j.at("delta_t").get_to(obj.delta_t);
      j.at("alpha_t").get_to(obj.alpha_t);
   }

   template <typename System>
   void to_json(nlohmann::json &j, loadcase<System> const &obj)
   {
      j = nlohmann::json{
          {"name", obj.name},
          {"node_load", obj.node_load},
          {"temperature_loads", obj.temperature_loads}};
   }

   template <typename System>
   void from_json(nlohmann::json const &j, loadcase<System> &obj)
   {
      j.at("name").get_to(obj.name);
      j.at("node_load").get_to(obj.node_load);
      j.at("temperature_loads").get_to(obj.temperature_loads);
   }
}
