#pragma once

#include <sec21/numeric/make_matrix.h>
#include <sec21/structural_analysis/concepts.h>

#include <memory>
#include <algorithm>

#ifdef __cpp_lib_format
   #include <format>
#endif

namespace sec21::structural_analysis::impl
{
   // template <typename System>
   // [[nodiscard]] auto coincidence_matrix(System const& sys, typename System::beam_t const& m)
   //    -> boost::numeric::ublas::matrix<int>
   // {}

   template <typename Allocator, SpaceTruss2D System>
   [[nodiscard]] auto coincidence_matrix(System const& sys, typename System::member_descriptor_t id)
   {
      constexpr auto dim = System::node_t::dimension_v;
      // static_assert(System::node_t::dimension_v == 2, "Only works with 2D system");

      auto find_node = [&sys](auto const& id)
      {
         const auto it = std::find_if(begin(sys.nodes), end(sys.nodes), [&id](auto&& e) { return id == e.id; });
         if (it == end(sys.nodes))
#ifdef __cpp_lib_format
            throw std::runtime_error(std::format("coincidence_matrix: could not find node id {}", id));
#else            
            throw std::runtime_error("coincidence_matrix: could not find node");
#endif
         return it;
      };

      const auto n = 4; // anzahl der verschiebungen 
      const auto m = std::size(sys.nodes) * dim;
      const auto[s, e] = sys.coincidence_table.at(id);
      const auto from = find_node(s);
      const auto to =  find_node(e);
      const auto u = std::distance(std::begin(sys.nodes), from);
      const auto v = std::distance(std::begin(sys.nodes), to);

      auto result = numeric::make_matrix<typename std::allocator_traits<Allocator>::value_type, Allocator>(n, m, 0);
      result(0, (u * dim) + 0) = 1;
      result(1, (u * dim) + 1) = 1;
      result(2, (v * dim) + 0) = 1;
      result(3, (v * dim) + 1) = 1;
      return result;
   }

   template <SpaceTruss2D System>
   [[nodiscard]] auto coincidence_matrix(System const& sys, typename System::member_descriptor_t id) 
   {
      return coincidence_matrix<std::allocator<int>>(sys, id);
   }
}