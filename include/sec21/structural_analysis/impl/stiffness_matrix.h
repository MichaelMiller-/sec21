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
      
      const auto it = std::find_if(begin(sys.members), end(sys.members), [&id](auto && e) { return id == e.id; }); 
      if (it == std::end(sys.members))
         throw std::runtime_error("stiffness_matrix: member not found");

      const auto n = std::size(sys.nodes) * dim;
      auto result = numeric::make_matrix<precision_t, Allocator>(n, n, 0);

      // using allocator_t = numeric::ublas_allocator_wrapper<std::pmr::polymorphic_allocator<double>>;
      using allocator_t = std::allocator<double>;

      auto K = steifigkeitsbeziehung_fachwerkstab_globalen_koordinaten(sys, it->id);
      auto Z = coincidence_matrix(sys, it->id);
      ublas::matrix<precision_t> Z_transposed = ublas::trans(Z);
      ublas::matrix<precision_t> K_1 = ublas::prod(Z_transposed, K);
      //! \todo use opb_prod() or axpy_prod() -> benchmark
      // ublas::opb_prod(ublas::prod(Z_transposed, K), Z, result, false);
      result = ublas::prod(K_1, Z);
      return result;
   }
   
   template <typename System>
   [[nodiscard]] auto stiffness_matrix(System const& sys, typename System::member_descriptor_t id) 
   {
      return stiffness_matrix<std::allocator<typename System::precision_t>>(sys, id);
   }   
}
