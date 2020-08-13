#pragma once

#include <sec21/structural_analysis/concepts.h>
#include <sec21/structural_analysis/node.h>
#include <sec21/structural_analysis/member.h>
#include <sec21/structural_analysis/descriptor_traits.h>
#include <sec21/structural_analysis/error_codes.h>

#include <boost/outcome.hpp>

#include <vector>
#include <algorithm>
#include <type_traits>
#include <map>

namespace sec21::structural_analysis
{
   namespace outcome = BOOST_OUTCOME_V2_NAMESPACE;

   //! \brief Datencontainer für das Stabwerkssystem
   //! \brief A space frame truss is a N-dimensional framework of members pinned at their ends
   struct space_truss
   {
      static constexpr int dimension_v = 2;

      using descriptor_t = int;
      //! \todo 2019-04-23 node and member as template parameter
      //! \todo 2019-04-27 precision_t als template parameter
      using precision_t = double;
      //! \todo node_descriptor == ->  template parameter
      using node_t = node<dimension_v, descriptor_t, precision_t>;
      //! \todo member descriptor == ->  template parameter
      using member_t = member<descriptor_t>;

      using node_descriptor_t = node_t::descriptor_t;
      using member_descriptor_t = member_t::descriptor_t;

      std::optional<std::string> description{};
      std::vector<node_t> nodes{};
      std::vector<member_t> members{};
      // member id -> start and end node
      std::map<member_descriptor_t, std::pair<node_descriptor_t, node_descriptor_t>>  coincidence_table{};
   };

   //! \return handle type
   template <typename System> 
   auto add_node(
      System& sys,
      typename System::node_t node) noexcept -> outcome::std_result<typename System::node_descriptor_t>
   {
      if (node.name == descriptor_traits<decltype(node.name)>::invalid())
         // return error_code::invalid_node_name;
         return std::errc::invalid_argument;

      if (std::find_if(begin(sys.nodes), end(sys.nodes), [&](auto && e) { return node.name == e.name; }) != std::end(sys.nodes))
         //! \todo error_code::node_already_exists
         return std::errc::invalid_argument;

      const auto result = sys.nodes.emplace_back(std::move(node));
      return result.name;
   }

   template <typename System, typename... Args>
   auto add_node(System& sys, Args &&... args) noexcept 
   {
      return add_node(sys, typename System::node_t{ std::forward<Args>(args)... });
   }

   template <typename System> //, typename... Args>
   auto add_member(
      System& sys,
      typename System::node_descriptor_t from,
      typename System::node_descriptor_t to,
      typename System::member_t member) noexcept -> outcome::std_result<typename System::member_descriptor_t>
   {
      if (member.name == descriptor_traits<typename System::member_descriptor_t>::invalid())
         //! \todo error_code::invalid_descriptor
         return std::errc::invalid_argument;

      const auto it_from = std::find_if(begin(sys.nodes), end(sys.nodes), [&from](auto && e) { return from == e.name; });
      const auto it_to = std::find_if(begin(sys.nodes), end(sys.nodes), [&to](auto && e) { return to == e.name; });

      if (it_from == std::end(sys.nodes) || it_to == std::end(sys.nodes))
         //! \todo error_code::node_not_found
         return std::errc::invalid_argument;

      if (std::find_if(begin(sys.members), end(sys.members), [&member](auto && e) { return member.name == e.name; }) != end(sys.members))
         //! \todo error_code::member_already_exists
         return std::errc::invalid_argument;

      const auto result = sys.members.emplace_back(std::move(member));

      sys.coincidence_table[result.name] = std::make_pair(from, to);
         // std::distance<decltype(it_from)>(std::begin(sys.nodes), it_from),
         // std::distance<decltype(it_to)>(std::begin(sys.nodes), it_to));

      return result.name;
   }

   template <typename System, typename... Args>
   auto add_member(
      System& sys,
      typename System::node_descriptor_t from,
      typename System::node_descriptor_t to,
      Args &&... args) noexcept -> outcome::std_result<typename System::member_descriptor_t>
   {
      return add_member(sys, from, to, { std::forward<Args>(args)... });
   }
}

#include <nlohmann/json.hpp>

namespace sec21::structural_analysis
{
   void to_json(nlohmann::json& j, space_truss const& obj) 
   {
      j = nlohmann::json{
         {"description", obj.description},
         {"nodes", obj.nodes},
         {"members", obj.members}, 
         {"connected", obj.coincidence_table}
      };
   }

   void from_json(nlohmann::json const& j, space_truss& obj) 
   {
      j.at("description").get_to(obj.description);
      j.at("nodes").get_to(obj.nodes);
      j.at("members").get_to(obj.members);
      j.at("connected").get_to(obj.coincidence_table);
   }
}