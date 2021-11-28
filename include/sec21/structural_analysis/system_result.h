#pragma once

#include <sec21/units/quantity.h>
#include <sec21/structural_analysis/common.h>

#include <vector>
#include <map>
#include <set>
#include <string_view>
#include <optional>

namespace sec21::structural_analysis
{
   template <typename Descriptor, typename Displacement, typename Force, auto Dimension>
   struct node_result_2
   {
      Descriptor id{};
      std::array<Displacement, Dimension> displacement{};
      std::array<Force, Dimension> support_reaction{};
   };

   template <typename Descriptor, typename Precision>
   struct member_result
   {
      using descriptor_t = Descriptor;
      using force_t = typename units::quantity<units::newton, Precision>;

      descriptor_t id{};
      force_t normal_force{};
   };

   template <typename System>
   struct system_result
   {
      static constexpr auto dimension_v = System::dimension_v;

      using precision_t = typename System::precision_t;
      using node_t = typename System::node_t;
      using member_t = typename System::member_t;
      using node_descriptor_t = typename node_t::descriptor_t;
      using member_descriptor_t = typename member_t::descriptor_t;

      using displacement_t = units::quantity<units::millimeter, precision_t>;
      using force_t = units::quantity<units::newton, precision_t>;

      struct node_result
      {
         std::array<displacement_t, dimension_v> displacement{};
         std::array<force_t, dimension_v> support_reaction{};
         //! \todo or boost::qvm
//         boost::qvm::vec<displacement_t, dimension_v> displacement{};
      };

      //! \todo decide which approch -> map forces to use structured bindings -> no RVO
      std::map<node_descriptor_t, node_result> node;

      using node_result_t = node_result_2<node_descriptor_t, displacement_t, force_t, dimension_v>;
      using member_result_set_t = member_result<member_descriptor_t, precision_t>;

      std::vector<node_result_t> nodes;
      std::vector<member_result_set_t> members;
   };

   template <typename Writer, typename System>
   void write_result(Writer& writer, system_result<System> const& result)
   {
      using namespace boost::qvm;

#if __cpp_lib_ranges
      std::ranges::for_each(result.nodes, [&writer](auto const& e) {
        writer.spacetruss_2D_insert_displacement(e.id, X(e.displacement).value(), Y(e.displacement).value());
        //! \todo write support reactions
      });
      std::ranges::for_each(result.members, [&writer](auto const& e) {
        writer.spacetruss_2D_insert_normal_force(e.id, e.normal_force.value());
      });
#else
      std::for_each(begin(result.nodes), end(result.nodes), [&writer](auto const& e) {
     writer.spacetruss_2D_insert_displacement(e.id, X(e.displacement).value(), Y(e.displacement).value());
     //! \todo write support reactions
   });
   std::for_each(begin(result.members), end(result.members), [&writer](auto const& e) {
     writer.spacetruss_2D_insert_normal_force(e.id, e.normal_force.value());
   });
#endif
   }

   //! \todo consider std::optional
   //! \todo not exception save!
   //! \todo argument displacements and reactions could be a view
   //! \todo return outcome
   template<typename System, typename T>
   auto make_result(System const& sys, std::vector<T> const& displacements, std::vector<T> const& support_reactions)
   -> std::optional<system_result<System>>
   {
      using precision_t = typename System::precision_t;

      if (size(displacements) != size(support_reactions))
         throw std::runtime_error("make_result: mismatch sizes");

      constexpr auto dim = System::dimension_v;

      system_result<System> result{};

      for (auto i = 0, j = 0; i < size(sys.nodes); ++i, j += dim) {
         result.nodes.push_back({sys.nodes[i].id, displacements[j], displacements[j + 1], support_reactions[j], support_reactions[j + 1]});
      }

      for (auto const& m : sys.members) {
         const auto lookup = sys.coincidence_table.at(m.id);

         const auto it_s =
            std::find_if(begin(result.nodes), end(result.nodes), [&lookup](auto n) { return n.id == lookup.first; });
         const auto it_e = std::find_if(begin(result.nodes), end(result.nodes),
                                        [&lookup](auto n) { return n.id == lookup.second; });

         const auto delta_s = it_s->displacement;
         const auto delta_e = it_e->displacement;
         const auto kv = EA_over_l(m.E, m.A, impl::length(sys, m.id)).value();
         const auto alpha = impl::angle_to_x_axis(sys, m.id);
         //! \todo k1?
         const auto k1 = alpha < 0.0 ? -1.0 : 1.0;

         using namespace boost::qvm;
         //! clang-format off
         const auto N = (
                           -std::cos(alpha) * X(delta_s).value() +
                           -std::sin(alpha) * Y(delta_s).value() +
                           std::cos(alpha) * X(delta_e).value() +
                           std::sin(alpha) * Y(delta_e).value()) * k1 * kv;
         //! clang-format on
         result.members.push_back({m.id, N});
      }
      // sanity check
//         if (size(result.members) != size(sys.members) || size(result.nodes) != size(sys.nodes))
//            return {};
      return result;
   }
}

#include <nlohmann/json.hpp>

namespace sec21::structural_analysis
{
   //
   template <typename Descriptor, typename Displacement, typename Force, auto Dimension>
   void to_json(nlohmann::json& j, node_result_2<Descriptor, Displacement, Force, Dimension> const& obj)
   {
      j = nlohmann::json{
         {"id", obj.id},
         {"displacement", obj.displacement}
      };
   }

   template <typename Descriptor, typename Displacement, typename Force, auto Dimension>
   void from_json(nlohmann::json const& j, node_result_2<Descriptor, Displacement, Force, Dimension>& obj)
   {
      j.at("id").get_to(obj.id);
      j.at("displacement").get_to(obj.displacement);
   }

   // 
   template <typename Descriptor, typename Force>
   void to_json(nlohmann::json& j, member_result<Descriptor, Force> const& obj) 
   {
      j = nlohmann::json{
         {"id", obj.id},
         {"normal_force", obj.normal_force}
      };
   }

   template <typename Descriptor, typename Force>
   void from_json(nlohmann::json const& j, member_result<Descriptor, Force>& obj) 
   {
      j.at("id").get_to(obj.id);
      j.at("normal_force").get_to(obj.normal_force);
   }

   //
   template <typename System>
   void to_json(nlohmann::json& j, system_result<System> const& obj) 
   {
      j = nlohmann::json{
         {"nodes", obj.nodes},
         {"members", obj.members}
      };
   }

   template <typename System>
   void from_json(nlohmann::json const& j, system_result<System>& obj) 
   {
      j.at("nodes").get_to(obj.nodes);
      j.at("members").get_to(obj.members);
   }
}
