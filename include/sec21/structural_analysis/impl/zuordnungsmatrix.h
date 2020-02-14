#pragma once

#include <boost/numeric/ublas/matrix.hpp>

namespace sec21::structural_analysis::impl
{
   template <typename System>
   [[nodiscard]] auto zuordnungsmatrix(System const& sys, typename System::member_descriptor_t id) 
      -> boost::numeric::ublas::matrix<int>
   {
      using namespace boost::numeric;

      constexpr auto dim = System::node_t::dimension_v;
      static_assert(dim == 2, "currently only works with 2D systems");

      const auto n = 4; // anzahl der verschiebungen 
      const auto m = std::size(sys.nodes) * dim;

      auto[s,e] = sys.adjacency_list.at(id);
      ublas::matrix<int> result(n, m, 0);
      result(0, (s * dim) + 0) = 1;
      result(1, (s * dim) + 1) = 1;
      result(2, (e * dim) + 0) = 1;
      result(3, (e * dim) + 1) = 1;
      return result;
   }   
}