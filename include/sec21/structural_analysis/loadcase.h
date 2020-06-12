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
   template <typename System>
   struct loadcase
   {
      static constexpr auto dimension_v = System::dimension_v;

      using precision_t = typename System::precision_t;
      using node_t = typename System::node_t;
      using member_t = typename System::member_t;
      using node_descriptor_t = typename node_t::descriptor_t;
      using member_descriptor_t = typename member_t::descriptor_t;

      using load_t = force_t<dimension_v>;
      using temperature_t = units::quantity<units::kelvin, precision_t>;

      std::string name{};
      //! \todo could be a std::variant
      std::vector<std::pair<node_descriptor_t, load_t>> node_load;
      std::vector<std::pair<member_descriptor_t, temperature_t>> temperature_load;
   };

   template <typename System, typename Unit>
   auto add_node_load(System const& sys, loadcase<System> const& load, std::vector<Unit>& F)
   {
      constexpr auto dim = System::dimension_v;
      //! \todo std::for_each(std::execution::par
      for (auto const& lf : load.node_load)
      {
         const auto it = std::find_if(
            begin(sys.nodes), 
            end(sys.nodes), 
            [id = lf.first](auto const& m){ return m.id == id; });

         if (it == end(sys.nodes))
            throw std::invalid_argument("invalid node id");

         const auto pos = std::distance(begin(sys.nodes), it) * dim;

         auto output_iterator = std::begin(F);
         std::advance(output_iterator, pos);

         std::copy(begin(lf.second), end(lf.second), output_iterator);
      }
   }

   template <typename System, typename Unit>
   auto add_temperature_load(System const& sys, loadcase<System> const& load, std::vector<Unit>& F)
   {
      constexpr auto dim = System::dimension_v;
      //! \todo std::for_each(std::execution::par
      for (auto const& lf : load.temperature_load)
      {
         const auto it = std::find_if(
            begin(sys.members), 
            end(sys.members), 
            [id = lf.first](auto const& m){ return m.id == id; });

         if (it == std::end(sys.members))
            throw std::invalid_argument("member id not found");

         const auto [s, e] = sys.coincidence_table.at(lf.first);
         const auto delta_t = lf.second.value();
         //! \todo remove magic number
         const auto alpha_t = 0.012;
         //! \todo use unit system; scale from kN -> N
         const auto equivalent_force = it->E.value() * it->A.value() * alpha_t * delta_t;

         const auto from = std::find_if(
            begin(sys.nodes), 
            end(sys.nodes), 
            [s](auto const& n){ return n.id == s; });

         const auto to = std::find_if(
            begin(sys.nodes), 
            end(sys.nodes), 
            [e](auto const& n){ return n.id == e; });

         const auto d1 = std::distance(std::begin(sys.nodes), from);
         const auto d2 = std::distance(std::begin(sys.nodes), to);
         //! \todo error handling

         const auto alpha = impl::angle_to_x_axis(*from, *to);
         //! \todo use unit system
         //! \todo use operator += 
         F[d1 * dim + 0] = std::cos(alpha) * -equivalent_force;
         F[d1 * dim + 1] = std::sin(alpha) * -equivalent_force;
         F[d2 * dim + 0] = std::cos(alpha) * equivalent_force;
         F[d2 * dim + 1] = std::sin(alpha) * equivalent_force;
      }
   }
}

#include <nlohmann/json.hpp>

namespace sec21::structural_analysis
{
   template <typename System>
   void to_json(nlohmann::json& j, loadcase<System> const& obj) 
   {
      j = nlohmann::json{
         {"name", obj.name},
         {"node_load", obj.node_load},
         {"temperature_load", obj.temperature_load}
      };
   }

   template <typename System>
   void from_json(nlohmann::json const& j, loadcase<System>& obj) 
   {
      j.at("name").get_to(obj.name);
      j.at("node_load").get_to(obj.node_load);
      j.at("temperature_load").get_to(obj.temperature_load);
   }
}
