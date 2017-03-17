#ifndef SEC21_STRUCTURAL_ANALYSIS_DETAIL_TRAITS_H
#define SEC21_STRUCTURAL_ANALYSIS_DETAIL_TRAITS_H

#include <type_traits>

#include <boost/optional.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/algorithm/string.hpp>

namespace sec21
{
namespace structural_analysis
{
namespace detail
{
   template<
      typename Graph,
      typename T> //! \todo enable_if
/*      ,
      typename std::enable_if<
         !std::is_same<
            typename boost::vertex_bundle_type<Graph>::type,
            boost::no_vertex_bundle>{}
      >::type* = nullptr>
*/      
   auto find_node(Graph & g, T const& t)
   {
      static const auto kNullVertex = boost::graph_traits<Graph>::null_vertex();

      typedef typename boost::vertex_bundle_type<Graph>::type vertex_property;
      typedef typename vertex_property::name_type node_name_type;

      const auto name_finder = [&](node_name_type const& vertex_name)
      {
         const auto p_map = boost::get(boost::vertex_bundle, g);
         for(auto it = boost::vertices(g); it.first != it.second; ++it.first)
         {
            //! \todo boost::iequals really best solution?
            if (boost::iequals(
               p_map[*it.first].name_, 
               vertex_name))
                  return *it.first;
         }
         // vertex not found
         return kNullVertex;
      };

      const auto node = name_finder(t);

      return boost::make_optional(node != kNullVertex, node);
   }

} // namespace detail
} // namespace structural_analysis
} // namespace sec21

#endif // SEC21_STRUCTURAL_ANALYSIS_DETAIL_TRAITS_H