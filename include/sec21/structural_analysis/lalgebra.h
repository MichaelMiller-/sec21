#ifndef SEC21_STRUCTURAL_ANALYSIS_FRAMEWORK_LALGEBRA_H
#define SEC21_STRUCTURAL_ANALYSIS_FRAMEWORK_LALGEBRA_H

// sec21
#include "force.h"
#include "framework.h"
// c++
#include <cstdint>
#include <vector>
#include <numeric>
#include <array>
#include <algorithm>
// boost
#include <boost/config.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/adjacency_list.hpp>

namespace sec21
{
namespace structural_analysis
{
namespace lalgebra
{
   //! \param n size of vector, if it is 0 then it will be calculated 
   //! \todo works only with 2D frameworks
   template<
      class Framework>
   auto generate_vector_b(Framework const& g, unsigned long n = 0)
   {
      using namespace sec21::structural_analysis;

      if (is_3D(g))
         throw std::runtime_error("only works with 2D frameworks");

      const auto dim = [&](){
         if (n == 0)
            return boost::num_edges(g) + num_support_reactions(g);
         else
            return n;
      }();

      if (dim % 2)
         throw std::runtime_error("invalid vector dimension");

      typedef boost::graph_traits<Framework>             graph_traits;
      typedef typename graph_traits::vertex_descriptor   vd_type;

      typedef typename boost::vertex_bundle_type<Framework>::type vertex_property;
      typedef typename vertex_property::force_type                force_type;
      typedef typename force_type::value_type                     value_type;

      const auto vertex_map = boost::get(boost::vertex_bundle, g);
      std::vector<value_type> b(dim, 0); // return value
      // counter
      typename std::vector<value_type>::size_type i = 0;

      //! \todo try to parallize !!! attention: share = i !!!
      std::for_each(
         boost::vertices(g).first,
         boost::vertices(g).second, 
         [&] (auto const& vi)
         {
            //! \todo write free function -> summize all forces at the node
            const auto result = [&]() -> force_type {
               auto res = force_type();
               for (auto const& f : vertex_map[vi].forces_)
                  res += f;
               return res;
            }();

            b[2 * i + 0] = [&](){ 
               if (std::isnormal(result[0]))
                  return result[0]; //! \todo necessarry * -1;
               }();

            b[2 * i + 1] = [&](){ 
               if (std::isnormal(result[1]))
                  return result[1]; //! \todo necessarry * -1;
               }();

            i++;
         });

      return b;
   } // generate_vector_b

   template<
      typename Framework>
   auto generate_matrix(Framework const& g, unsigned long n = 0)
   {
      using namespace sec21::structural_analysis;

      const auto dim = [&](){
         if (n == 0)
            return boost::num_edges(g) + num_support_reactions(g);
         else
            return n;
      }();

      if (dim % 2)
         throw std::runtime_error("invalid matrix dimension");

      using namespace sec21::math::matrix;

      typedef unsigned long index_type;
      // bars from node to node (std::pair<from, to>)
      //! \todo replace or test with boost::edges(g)
      typedef std::pair<index_type, index_type> bars_type;
      //! \todo replace (test) with std::unordered_multimap
      typedef std::vector<bars_type> vector_bars_type;

      typedef typename boost::graph_traits<Framework>::edge_descriptor   edge_descriptor_type;

      const auto vertex_id = boost::get(boost::vertex_index, g);

      auto node_to_bars = std::map<index_type, vector_bars_type>();
      auto i = index_type(0);

      std::for_each(
         boost::edges(g).first,
         boost::edges(g).second,
         [&](edge_descriptor_type const& edge)
         {
            const auto src = boost::get(vertex_id, boost::source(edge, g)); 
            const auto trg = boost::get(vertex_id, boost::target(edge, g));

            node_to_bars[src].emplace_back(vector_bars_type::value_type(i, trg)); 
            node_to_bars[trg].emplace_back(vector_bars_type::value_type(i, src));          

            i++;
         }
      );

      typedef typename boost::graph_traits<Framework>::vertex_descriptor   vd_type;
      typedef typename boost::vertex_bundle_type<Framework>::type          vertex_property_type;
      typedef typename vertex_property_type::vector_type                   vector_type;
      typedef typename vertex_property_type::force_type                    force_type;
      typedef typename force_type::value_type                              value_type;

      const auto num_reactions = num_support_reactions(g);
      const auto p_map = boost::get(boost::vertex_bundle, g);

      auto node_pos_lookup = std::vector<vector_type>();
      auto m = std::vector<value_type>(dim * dim, 0.); // return_value

      //! \brief unbekannte auflagerkraft pro achse
      struct external_force
      {
         external_force(
            unsigned long n,
            unsigned long dir_index) BOOST_NOEXCEPT : node_number_(n)
         { 
            axis_.fill(0.);
            axis_[dir_index] = 1.;
         }

         unsigned long node_number_;
         std::array<float, 3> axis_;
      };
      std::vector<external_force> external_forces; // (num_reactions);

      std::for_each(
         boost::vertices(g).first,
         boost::vertices(g).second, 
         [&] (vd_type const& vi)
         {
            // collect all node positions
            node_pos_lookup.push_back(p_map[vi].world_pos_);

            // collect unkown external forces (supports) for every axis
            const auto & supp = p_map[vi].support_;
            if (supp)
            {
               const auto reactions = supp->reactions();
               // const auto reactions_vectors = supp->forces();

               if (reactions == 1) // keine kombination
                  external_forces.emplace_back(vi, 1);
               else
                  // anzahl der kombination
                  for (auto i = 0; i < reactions; ++i)
                     external_forces.emplace_back(vi, i);
            }
         }
      );

      auto force_idx = boost::num_vertices(g) + num_reactions;
      for (auto & e : external_forces)
      {
         m[idx_row_major_array(2 * e.node_number_ + 0, force_idx, dim)] = e.axis_[0];
         m[idx_row_major_array(2 * e.node_number_ + 1, force_idx, dim)] = e.axis_[1];
         force_idx++;
      }

      using namespace sec21::math::matrix;
      // fill matrix
      index_type n1 = 0ul;
      for (auto & a : node_to_bars)
      {
         for (auto & b : a.second)
         {
            const auto dir = normalized(node_pos_lookup[b.second] - node_pos_lookup[n1]);

            m[idx_row_major_array(2 * a.first + 0, b.first, dim)] = dir[0];
            m[idx_row_major_array(2 * a.first + 1, b.first, dim)] = dir[1];
         }
         n1++;
      }

      return m;
   }
} // namespace lalgebra
} // namespace structural_analysis
} // namespace sec21

#ifdef SEC21_USE_LUA

#include <LuaIntf/LuaIntf.h>

namespace sec21
{
   template<
      typename FrameworkType>
   static void register_lua_lalgebra(lua_State* L, FrameworkType& g)
   {
      using namespace LuaIntf;
      using namespace sec21::structural_analysis::lalgebra;

      LuaBinding(L).beginModule("framework")
         .addFunction(
            "generate_vector_b", [&]() { return generate_vector_b<float>(g); })
         .addFunction(
            "generate_matrix", [&]() { return generate_matrix<float>(g); })
      .endModule();
   } // register_lua_lalgebra
} // namespace sec21

#endif // SEC21_USE_LUA


#endif
