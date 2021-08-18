#pragma once

#include <sec21/numeric/make_matrix.h>
#include <sec21/structural_analysis/impl/coincidence_matrix.h>
#include <sec21/structural_analysis/impl/steifigkeitsbeziehung_fachwerkstab_globalen_koordinaten.h>

#include <boost/numeric/ublas/matrix.hpp>

namespace sec21::structural_analysis::impl
{
   template <typename Allocator, typename System>
   [[nodiscard]] auto stiffness_matrix(System const& sys, typename System::member_descriptor_t id)
   {
      using namespace boost::numeric;

      constexpr auto dim = System::node_t::dimension_v;
      static_assert(System::node_t::dimension_v == 2, "Only works with 2D system");

      using precision_t = typename System::precision_t;

      const auto it = std::find_if(begin(sys.members), end(sys.members), [&id](auto&& e) { return id == e.id; });
      if (it == std::end(sys.members))
         throw std::runtime_error("stiffness_matrix: member not found");

      const auto n = std::size(sys.nodes) * dim;
      auto result = numeric::make_matrix<precision_t, Allocator>(n, n, 0);
      auto K = steifigkeitsbeziehung_fachwerkstab_globalen_koordinaten<Allocator>(sys, it->id);
      auto Z = coincidence_matrix<Allocator>(sys, it->id);
      decltype(result) K_1 = ublas::prod(ublas::trans(Z), K);

      result = ublas::prod(K_1, Z);
      return result;
   }
} // namespace sec21::structural_analysis::impl
