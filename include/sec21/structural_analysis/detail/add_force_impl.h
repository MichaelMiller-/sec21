#ifndef SEC21_STRUCTURAL_ANALYSIS_DETAIL_ADD_FORCE_IMPL_H
#define SEC21_STRUCTURAL_ANALYSIS_DETAIL_ADD_FORCE_IMPL_H

#include <boost/graph/properties.hpp>
#include <boost/graph/adjacency_list.hpp>

#include "traits.h"

namespace sec21
{
namespace structural_analysis
{
namespace detail
{
   template<
      typename Graph>/*,
      typename std::enable_if<
         !std::is_same<
            typename boost::vertex_bundle_type<Graph>::type,
            boost::no_vertex_bundle>{}
      >::type* = nullptr>*/
   struct add_force_impl
   {
      typedef typename boost::vertex_bundle_type<Graph>::type vertex_property;
      typedef typename vertex_property::name_type node_name_type;
      typedef typename vertex_property::force_type force_type;

      template<
         typename T,
         typename U>/*,
         typename std::enable_if<
               std::is_convertible<T,node_name_type>{} &&
               std::is_convertible<U,force_type>{}
            >::type* = nullptr>*/
      static void apply(
         Graph &g,
         T const& name,
         U const& force)
      {
         const auto node = find_node(g, name);

         if (!node)
            throw std::runtime_error("vertex not found");

         g[*node].forces_.push_back(force);
      }
   };
} // namespace detail
} // namespace structural_analysis
} // namespace sec21

#endif