#pragma once

#include <boost/numeric/ublas/matrix.hpp>

namespace sec21::structural_analysis::impl
{
   template <typename System>
   [[nodiscard]] auto coincidence_matrix(System const& sys, typename System::member_descriptor_t id) 
      -> boost::numeric::ublas::matrix<int>
   {
      using namespace boost::numeric;

      constexpr auto dim = System::node_t::dimension_v;
      static_assert(System::node_t::dimension_v == 2, "Only works with 2D system");

      const auto n = 4; // anzahl der verschiebungen 
      const auto m = std::size(sys.nodes) * dim;
      const auto[s, e] = sys.coincidence_table.at(id);
      const auto from = get_element(sys.nodes, s);
      const auto to = get_element(sys.nodes, e);  
      const auto u = std::distance(std::begin(sys.nodes), from);
      const auto v = std::distance(std::begin(sys.nodes), to);

      ublas::matrix<int> result(n, m, 0);
      result(0, (u * dim) + 0) = 1;
      result(1, (u * dim) + 1) = 1;
      result(2, (v * dim) + 0) = 1;
      result(3, (v * dim) + 1) = 1;
      return result;
   }   
}