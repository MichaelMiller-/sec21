#pragma once

#include <sec21/structural_analysis/node.h>
#include <sec21/structural_analysis/member.h>

//! \todo remove
#include <sec21/structural_analysis/impl/geometry.h>

#include <sec21/units/dimensions/length.h>

//! \remember outcome is implicit nodiscard
#include <boost/version.hpp>
#if BOOST_VERSION > 106900
#include <boost/outcome.hpp>
namespace outcome = BOOST_OUTCOME_V2_NAMESPACE;
#else
#include <outcome.hpp>
namespace outcome = OUTCOME_V2_NAMESPACE;
#endif

#include <boost/math/constants/constants.hpp>
#include <boost/math/special_functions/sin_pi.hpp>
#include <boost/math/special_functions/cos_pi.hpp>
#include <boost/geometry/algorithms/distance.hpp> 
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/io.hpp>

#include <vector>
// #include <map>
#include <algorithm>
#include <type_traits>

namespace sec21::structural_analysis
{
   enum class error_code
   {
      invalid_node_id,
      node_already_exists
   };

   template <typename T>
   using expected_t = outcome::std_result<T>;

   //! \brief Datencontainer für das Stabwerkssystem
   //! \brief A space frame truss is a three-dimensional framework of members pinned at their ends
   struct space_truss
   {
      constexpr static int dimension_v = 2;

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

      //! \todo 2019-08-26: possible to implement a node_iteraotr with types like descriptor, value_type
      using node_iterator_t = decltype(nodes)::iterator;
      using member_iterator_t = decltype(members)::iterator;
      using diff_t = typename decltype(nodes)::difference_type;
      
      // Koinzidenztabelle: stab id -> anfangs- und endknoten
      // coincidence-table: member id -> start and end node
      std::map<member_descriptor_t, std::pair<diff_t, diff_t>>  adjacency_list{};
   };

   template <typename T>
   struct is_space_truss : std::is_same<T, space_truss> {};

#ifdef __cpp_concepts
   template <typename T>
   concept SpaceTruss = is_space_truss<T>::value;
#endif

   namespace detail
   {
      template <typename T>
      constexpr bool has_invalid_id(T const& v) noexcept {
         return v.id == std::numeric_limits<decltype(v.id)>::max();
      }

      template <typename Container, typename Descriptor>
      constexpr auto get_element(Container const& c, Descriptor id) noexcept {
         return std::find_if(std::begin(c), std::end(c), [&id](auto && e) { return id == e.id; });
      }
   }

   //! \return handle type
   template <typename System>
#ifdef __cpp_concepts
      requires SpaceTruss<System>
#endif
   auto add_node(
      System& sys,
      typename System::node_t node) noexcept -> expected_t<typename System::node_descriptor_t>
   {
      if (detail::has_invalid_id(node))
         //! \todo error_code::node_invalid_id
         return std::errc::invalid_argument;

      if (detail::get_element(sys.nodes, node.id) != std::end(sys.nodes))
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
      typename System::member_t member) noexcept -> expected_t<typename System::member_descriptor_t>
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

      sys.adjacency_list[result.id] = std::make_pair(
         std::distance<decltype(it_from)>(std::begin(sys.nodes), it_from),
         std::distance<decltype(it_to)>(std::begin(sys.nodes), it_to));

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
      Args &&... args) noexcept -> expected_t<typename System::member_descriptor_t>
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
      const auto m{ sys.adjacency_list.at(id) };
      //! \todo possible exception
      return { boost::geometry::distance(sys.nodes.at(m.first).position, sys.nodes.at(m.second).position) };
   }
}
