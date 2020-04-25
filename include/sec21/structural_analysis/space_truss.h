#pragma once

#include <sec21/structural_analysis/node.h>
#include <sec21/structural_analysis/member.h>
#include <sec21/structural_analysis/type_traits.h>

//! \todo remove
// #include <sec21/structural_analysis/impl/geometry.h>

#include <sec21/units/dimensions/length.h>

#include <boost/version.hpp>
#include <boost/outcome.hpp>
#include <boost/math/constants/constants.hpp>
#include <boost/math/special_functions/sin_pi.hpp>
#include <boost/math/special_functions/cos_pi.hpp>
#include <boost/geometry/algorithms/distance.hpp> 
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/io.hpp>

#include <vector>
#include <algorithm>
#include <type_traits>
#include <map>

namespace sec21::structural_analysis
{
   namespace outcome = BOOST_OUTCOME_V2_NAMESPACE;

   //! \todo implement/use
   enum class error_code
   {
      invalid_node_id,
      node_already_exists
   };

   //! \brief Datencontainer für das Stabwerkssystem
   //! \brief A space frame truss is a N-dimensional framework of members pinned at their ends
   struct space_truss
   {
      static constexpr int dimension_v = 2;

      //! \todo 2019-04-23 node and member as template parameter
      //! \todo 2019-04-27 precision_t als template parameter
      using precision_t = double;
      using node_t = node<dimension_v>;
      using member_t = member;

      using node_descriptor_t = node_t::descriptor_t;
      using force_t = node_t::load_t;
      using member_descriptor_t = member_t::descriptor_t;

      std::vector<node_t>  nodes;
      std::vector<member_t>  members;
      
      // member id -> start and end node
      std::map<member_descriptor_t, std::pair<node_descriptor_t, node_descriptor_t>>  coincidence_table{};
   };

   inline namespace detail
   {
      template <typename T>
      constexpr bool has_invalid_id(T const& v) noexcept 
      {
         //! \todo check via customization point
         return v.id == std::numeric_limits<decltype(v.id)>::max();
      }

      template <typename Container, typename Descriptor>
      constexpr auto get_element(Container const& c, Descriptor id) noexcept 
      {
         return std::find_if(std::begin(c), std::end(c), [&id](auto && e) { return id == e.id; });
      }

      template <typename Container, typename Descriptor>
      constexpr bool valid_descriptor(Container&& c, Descriptor id) noexcept 
      {
         return std::find_if(std::begin(c), std::end(c), [&id](auto && e) { return id == e.id; }) != std::end(c);
      }
   }

   //! \return handle type
   template <typename System>
#ifdef __cpp_concepts
      requires SpaceTruss<System>
#endif
   auto add_node(
      System& sys,
      typename System::node_t node) noexcept -> outcome::std_result<typename System::node_descriptor_t>
   {
      if (detail::has_invalid_id(node))
         //! \todo error_code::node_invalid_id
         return std::errc::invalid_argument;

      if (detail::valid_descriptor(sys.nodes, node.id))
         //! \todo error_code::node_already_exists
         return std::errc::invalid_argument;

      const auto result = sys.nodes.emplace_back(std::move(node));
      return result.id;
   }

   template <typename System, typename... Args>
#ifdef __cpp_concepts
      requires SpaceTruss<System>
#endif
   auto add_node(System& sys, Args &&... args) noexcept 
   {
      return add_node(sys, typename System::node_t{ std::forward<Args>(args)... });
   }

   //! \todo 2019-04-15 check something like "system_traits" -> see graph_traits from boost
   template <typename System> //, typename... Args>
#ifdef __cpp_concepts
      requires SpaceTruss<System>
#endif
   auto add_member(
      System& sys,
      typename System::node_descriptor_t from,
      typename System::node_descriptor_t to,
      typename System::member_t member) noexcept -> outcome::std_result<typename System::member_descriptor_t>
   {
      if (detail::has_invalid_id(member))
         //! \todo error_code::invalid_id
         return std::errc::invalid_argument;

      const auto it_from = detail::get_element(sys.nodes, from);
      const auto it_to = detail::get_element(sys.nodes, to);

      if (it_from == std::end(sys.nodes) || it_to == std::end(sys.nodes))
         //! \todo error_code::node_not_found
         return std::errc::invalid_argument;

      if (detail::get_element(sys.members, member.id) != std::end(sys.members))
         //! \todo error_code::member_already_exists
         return std::errc::invalid_argument;

      const auto result = sys.members.emplace_back(std::move(member));

      sys.coincidence_table[result.id] = std::make_pair(from, to);
         // std::distance<decltype(it_from)>(std::begin(sys.nodes), it_from),
         // std::distance<decltype(it_to)>(std::begin(sys.nodes), it_to));

      return result.id;
   }

   template <typename System, typename... Args>
#ifdef __cpp_concepts
      requires SpaceTruss<System>
#endif   
   auto add_member(
      System& sys,
      typename System::node_descriptor_t from,
      typename System::node_descriptor_t to,
      Args &&... args) noexcept -> outcome::std_result<typename System::member_descriptor_t>
   {
      return add_member(sys, from, to, { std::forward<Args>(args)... });
   }

   //! \todo 2019-04-16 error handling -> outcome
   template <typename System>
#ifdef __cpp_concepts
      requires SpaceTruss<System>
#endif
   auto length(System const& sys, typename System::member_descriptor_t id) -> units::quantity<units::meter, typename System::precision_t>
   {
      //! \todo possible exception
      const auto [from, to] = sys.coincidence_table.at(id);
      const auto it_from = get_element(sys.nodes, from);
      const auto it_to = get_element(sys.nodes, to);
      //! \todo possible exception
      return { boost::geometry::distance(it_from->position, it_to->position) };
   }
}

//! \todo temporary; guard with a define or move to extra file
#include <nlohmann/json.hpp>

namespace sec21::structural_analysis
{
   void to_json(nlohmann::json& j, space_truss const& obj) {
      j = nlohmann::json{
         {"nodes", obj.nodes},
         {"members", obj.members}, 
         {"connected", obj.coincidence_table}
      };
   }

   void from_json(nlohmann::json const& j, space_truss& obj) 
   {
      j.at("nodes").get_to(obj.nodes);
      j.at("members").get_to(obj.members);
      j.at("connected").get_to(obj.coincidence_table);
   }
}