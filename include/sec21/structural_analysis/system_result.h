#pragma once

#include <sec21/units/quantity.h>

#include <vector>
#include <map>
#include <set>
#include <string_view>

namespace sec21::structural_analysis
{
   template <typename Descriptor, typename Displacement, typename Force, auto Dimension>
   struct node_result_2
   {
      Descriptor id{};
      std::array<Displacement, Dimension> displacement{};
      std::array<Force, Dimension> support_reaction{};
   };   

   struct node_result_transparent_comparator
   {
      using is_transparent = void;

      template <typename Descriptor, typename Displacement, typename Force, auto Dimension>
      bool operator()(
         node_result_2<Descriptor, Displacement, Force, Dimension> const& lhs, 
         node_result_2<Descriptor, Displacement, Force, Dimension> const& rhs) const noexcept
      {
         return lhs.id < rhs.id;
      }
      template <typename Descriptor, typename Displacement, typename Force, auto Dimension>
      bool operator()(
         Descriptor const& lhs, 
         node_result_2<Descriptor, Displacement, Force, Dimension> const& rhs) const noexcept
      {
         return lhs < rhs.id;
      }
      template <typename Descriptor, typename Displacement, typename Force, auto Dimension>
      bool operator()(
         node_result_2<Descriptor, Displacement, Force, Dimension> const& lhs, 
         Descriptor const& rhs) const noexcept
      {
         return lhs.id < rhs;
      }
   };
   
   template <typename Descriptor, typename Force = units::quantity<units::newton, double> >
   struct member_result_2
   {
      Descriptor id{};
      Force normal_force{};

      constexpr bool operator < (member_result_2 const& rhs) const
      {
         return id < rhs.id;
      }
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
         // boost::qvm::vec<displacement_t, dimension_v> displacement{};
      };
      struct member_result
      {
         force_t normal_force{};
      };

      //! \todo decide which approch
      std::map<node_descriptor_t, node_result> node;
      std::map<member_descriptor_t, member_result> member;

      using node_result_t = node_result_2<node_descriptor_t, displacement_t, force_t, dimension_v>;
      using member_result_t = member_result_2<member_descriptor_t, force_t>;

      std::set<node_result_t, node_result_transparent_comparator> nodes;
      std::set<member_result_t> members;
   };
}

//! \todo temporary; guard with a define or move to extra file
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
   void to_json(nlohmann::json& j, member_result_2<Descriptor, Force> const& obj) 
   {
      j = nlohmann::json{
         {"id", obj.id},
         {"normal_force", obj.normal_force}
      };
   }

   template <typename Descriptor, typename Force>
   void from_json(nlohmann::json const& j, member_result_2<Descriptor, Force>& obj) 
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
