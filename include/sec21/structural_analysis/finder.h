#pragma once
// boost
#include <boost/optional/optional.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/adjacency_list.hpp>

namespace sec21 {
namespace structural_analysis {

//! \todo graoh must have vertex_bundle_type
//! \todo typename std::enable_if<boost::has_bundled_vertex_property<Graph>::type>::type* = nullptr>
template<
   typename Graph>
struct find_by_property
{
   typedef typename boost::vertex_bundle_type<Graph>::type  vertex_property;

   find_by_property(Graph const& g) : graph_(g) { }

   // template <typename A, typename B, A B::*member>
   // std::string parameter value must be T(vertex_property::*ptr)
   template<
      typename T>
   auto get(T t, std::string(vertex_property::*ptr)) const
   {
      static const auto kNullVertex = boost::graph_traits<Graph>::null_vertex();

      const auto finder = [&](T const& prop)
      {
         const auto p_map = boost::get(ptr, graph_);

         for(auto it = boost::vertices(graph_); it.first != it.second; ++it.first)
            if (p_map[*it.first] == prop)
               return *it.first;
         // vertex not found
         return kNullVertex;
      };

      const auto node = finder(t);

      return boost::make_optional(node != kNullVertex, node);
   }

private:
   Graph const&   graph_;
};

//! \todo graph must have vertex_bundle_type
// typename std::enable_if<boost::has_bundled_vertex_property<Graph>::type>::type* = nullptr>
template<
   typename Graph> 
auto make_finder(Graph& g) { 
   return find_by_property<Graph>(g);
} // make_finder

} // structural_analysis
} // sec21
