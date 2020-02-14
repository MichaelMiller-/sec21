#pragma once

#include <sec21/structural_analysis/impl/zuordnungsmatrix.h>
#include <sec21/structural_analysis/impl/steifigkeitsbeziehung_fachwerkstab_globalen_koordinaten.h>

#include <boost/numeric/ublas/matrix.hpp>

namespace sec21::structural_analysis::impl
{
   template <typename System>
   [[nodiscard]] auto stiffness_matrix(System const& sys, typename System::member_descriptor_t id) 
      -> boost::numeric::ublas::matrix<typename System::precision_t>
   {
      using namespace boost::numeric;

      constexpr auto dim = System::node_t::dimension_v;
      static_assert(dim == 2, "currently only works with 2D systems");

      using precision_t = typename System::precision_t;

      const auto n = std::size(sys.nodes) * dim;
      ublas::matrix<precision_t> result(n, n, 0);

      if (const auto it = std::find_if(std::begin(sys.members), std::end(sys.members), [&id](auto && e) { return id == e.id; }); it != std::end(sys.members))
      {
         auto K = steifigkeitsbeziehung_fachwerkstab_globalen_koordinaten(sys, it->id);
         auto Z = zuordnungsmatrix(sys, it->id);
         ublas::matrix<precision_t> Z_transposed = boost::numeric::ublas::trans(Z);
         ublas::matrix<precision_t> K_1 = ublas::prod(Z_transposed, K);
         //! \todo use opb_prod() or axpy_prod()
         // ublas::opb_prod(ublas::prod(Z_transposed, K), Z, result, false);
         result = ublas::prod(K_1, Z);
      }
      return result;
   }
}