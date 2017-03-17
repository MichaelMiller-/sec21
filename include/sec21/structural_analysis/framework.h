#pragma once

// sec21
#include "node.h"
#include "rod2.h"
// c++
#include <initializer_list>
#include <exception>
#include <vector>
#include <numeric>
// boost
#include <boost/graph/adjacency_list.hpp>
#include <boost/qvm/vec_operations.hpp>
#include <boost/qvm/vec_access.hpp>
// ranges
#include <range/v3/all.hpp>

namespace sec21 {
namespace structural_analysis {

//! \todo name: truss
using framework_t = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, node, rod2>;

//! \moveable version
inline auto add_node(framework_t& g, framework_t::vertex_property_type node)
{
   //return boost::graph_traits<framework_t>::null_vertex();
   return boost::add_vertex(node, g);
}

inline auto add_node(
   framework_t& g, 
   decltype(node::position) const& pos, 
   std::initializer_list<decltype(node::forces)::value_type> force_list)
{
   node tmp;

   //return boost::add_vertex({ pos, boost::optional{}, force_list }, g);
}

inline auto add_rod2(
   framework_t& g,
   framework_t::vertex_descriptor const& from,
   framework_t::vertex_descriptor const& to,
   framework_t::edge_property_type rod2)
{
   framework_t::edge_descriptor ed;
   bool inserted;

   std::tie(ed, inserted) = boost::add_edge(from, to, rod2, g);

   if (!inserted)
      throw std::runtime_error("add_rod2(): couldn't add rod2 to framework");

   return ed;
}

inline auto add_support(
   framework_t& g,
   framework_t::vertex_descriptor const& node,
   support s)
{

}

inline auto get_supports(framework_t const& g)
{
   const auto vertices = boost::vertices(g);
   //! \todo ranges with first and second
   //! \todo init with size
   std::vector<support> lst;

   std::transform(vertices.first, vertices.second, std::back_inserter(lst),
      [&](auto vd) {
         if (g[vd].support) return *g[vd].support;
   });

   return lst;
} 

auto get_supports() {
   ranges::view::transform([&](auto vd) { if (g[vd].support) return *g[vd].support; });
}


inline auto add_force(framework_t& g, framework_t::vertex_descriptor const& node, force f) {
   return g[node].forces.push_back(f);
}

inline auto num_support_reactions(framework_t const& g)
{
   //! \todo ranges with boost::vertices(g) | get_support()
   const auto supports = get_supports(g);

   return std::accumulate(std::begin(supports), std::end(supports), 0,
      [](auto init, auto s) { 
         return init + reactions(s);
   });
}

//! \return n = a + s - 2k; n times statically determined; n == -1
inline int determined(framework_t const& g)
{
   const auto k = boost::num_vertices(g);

   if (k == 0) 
      return -1;

   const auto a = num_support_reactions(g);
   const auto s = boost::num_edges(g);

   return (a + s - 2 * k);
}

//! \todo only works with 2D 
inline auto matrix_A(framework_t const& g) 
{
   if (auto n = determined(g))
      throw std::runtime_error(n + " times statically determined");

   //! \todo implement
   using namespace boost::qvm;

   const auto dim = [](auto const& g) { return boost::num_edges(g) + num_support_reactions(g); }(g);

   using index_type = std::size_t;
      //! \todo float precision
      typedef std::array<float, 4>  vec4f_type;

      using traits = boost::graph_traits<framework_t>;
      using node_type = boost::vertex_bundle_type<framework_t>::type;
      using edge_type = boost::edge_bundle_type<framework_t>::type;

   // graph property map lookup
   const auto vertex_indices_map = boost::get(boost::vertex_index, g); // buildin indices
   const auto vertex_property_map = boost::get(boost::vertex_bundle, g);
      // const auto node_pos_map = boost::get(&node_type::world_pos_, g); //! \todo neccessary?
      const auto edge_name_map = boost::get(&edge_type::name, g);

      // property map lookup

      //! \todo async const auto future_edge_name2idx_map = std::async([&]() { }));
      //! \todo name to index map should be public
      // edge name (num edges + num support reactions) -> index in vector b
      const auto name2idx_map = [&]() {

         traits::edge_iterator it_edge, it_edge_end;
         auto cnt = index_type{ 0 };
         std::unordered_map<decltype(edge_type::name), index_type> result{ boost::num_edges(g) + 1 }; // add one additional bucket for performance

         for (
            boost::tie(it_edge, it_edge_end) = boost::edges(g);
            it_edge != it_edge_end;
            ++it_edge)
         {
            result.insert(std::make_pair(edge_name_map[*it_edge], cnt++));
         }
         return result;
      }();

      traits::out_edge_iterator it_out_edge, it_out_edge_end;
   auto i = std::size_t{ 0 };
   auto j = std::size_t{ 0 };

      auto num_reactions = 3; //! \todo remove
      auto force_idx = boost::num_vertices(g) + num_reactions;

   auto result = std::vector<float>(dim * dim, 0);
   
   using vec2_f = boost::qvm::vec<float, 2>;

   auto reactions_vector = [](support s)
   {
      if (s == support::MOVABLE)
         return vec2_f{ 0, 1 };

      if (s == support::HINGED)
         return vec2_f{ 1, 1 };

      return vec2_f{ 0,0 };
   };

   auto support_index = boost::num_edges(g);

   std::for_each(
      boost::vertices(g).first,
      boost::vertices(g).second,
      [&](auto const& vd)
   {
      auto const current_node_pos = vertex_property_map[vd].position;

      // iterate over node connected edges
      for (
         boost::tie(it_out_edge, it_out_edge_end) = boost::out_edges(vd, g);
         it_out_edge != it_out_edge_end; ++it_out_edge)
      {
         auto const edge_name = edge_name_map[*it_out_edge];
         auto const col = name2idx_map.at(edge_name);
         auto const trg = boost::get(vertex_indices_map, boost::target(*it_out_edge, g));
         auto const tmp_dir = normalized(g[trg].position - current_node_pos);

         result[(i + 0) * dim + col] = X(tmp_dir);
         result[(i + 1) * dim + col] = Y(tmp_dir);
      }

      // consider external forces (reactions) from support
      auto const supp = vertex_property_map[vd].support;

      if (supp)
      {
         auto const f = reactions_vector(*supp);
         //! \todo get support index in X, Y direction
         auto nn = (i + 0) * dim + boost::num_edges(g) + (j + 0);
         auto nn1 = (i + 1) * dim + boost::num_edges(g) + (j + 1);

         auto tt = (i + 0) * dim + support_index + 0;
         auto tt1 = (i + 1) * dim + support_index + 1;

         assert(nn == tt);
         assert(nn1 == tt1);

         result[(i + 0) * dim + boost::num_edges(g) + (j + 0)] = X(f);
         result[(i + 1) * dim + boost::num_edges(g) + (j + 1)] = Y(f);

         j += 2;
         support_index += 2;
      }

      i += 2;
   });
   return result;
}

//! \todo generic
//! \todo is only for 2D
auto vector_b(framework_t const& g)
{
   //if (is_3D(g))
   //   throw std::runtime_error("only works with 2D frameworks");

   if (auto n = determined(g))
      throw std::runtime_error(n + " times statically determined");

   const auto dim = [](auto const& g) { return boost::num_edges(g) + num_support_reactions(g); }(g);

   if (dim % 2)
      throw std::runtime_error("invalid vector dimension");

   const auto vertex_map = boost::get(boost::vertex_bundle, g);
   std::vector<float> b(dim, 0); //! \todo return value

   auto i = 0ul;
   //! \todo for_each_indexed
   std::for_each(
      boost::vertices(g).first,
      boost::vertices(g).second,
      [&](auto const& vd)
   {
      const auto res = std::accumulate(
         std::begin(vertex_map[vd].forces), 
         std::end(vertex_map[vd].forces), 
         decltype(vertex_map[vd].forces)::value_type{ 0, 0 });

      using namespace boost::qvm;

      b[2 * i + 0] = X(res);
      b[2 * i + 1] = Y(res);

      i++;
   });
   return b;
}


} // namespace structural_analysis
} // namespace sec21
