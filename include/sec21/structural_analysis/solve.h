#pragma once

#include <sec21/units.h>
#include <sec21/zip.h>
#include <sec21/erase_if_index.h>
#include <sec21/for_each_chunk.h>
#include <sec21/row_col.h>
#include <sec21/numeric/drop.h>
#include <sec21/numeric/ublas_allocator_wrapper.h>
#include <sec21/numeric/make_vector.h>
#include <sec21/units/quantity.h>
#include <sec21/structural_analysis/common.h>
#include <sec21/structural_analysis/loadcase.h>
#include <sec21/structural_analysis/system_result.h>
#include <sec21/structural_analysis/impl/lookup.h>
#include <sec21/structural_analysis/impl/geometry.h>
#include <sec21/structural_analysis/impl/global_stiffness_matrix.h>
//! \todo backend as template parameter
#include <sec21/structural_analysis/solver/backend/viennacl.h>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
//! \todo consider: <boost/numeric/ublas/opencl/operation.hpp>
#include <boost/numeric/ublas/operation.hpp>

#include <string_view>
#include <array>
#include <vector>
#include <memory_resource>

namespace sec21::structural_analysis
{
   namespace impl
   {
      template <typename Solver, typename System, typename Allocator>
      [[nodiscard]] auto solve(
         System const &sys, 
         boost::numeric::ublas::matrix<typename System::precision_t, boost::numeric::ublas::row_major, Allocator> K,
         boost::numeric::ublas::vector<typename System::precision_t, Allocator> F)
      {
         auto lookup = impl::make_lookup(sys, row{0});

         std::vector<bool> mask{};
         support_mask(std::begin(sys.nodes), std::end(sys.nodes), std::back_inserter(mask));

         decltype(lookup) supported_rows{};
         decltype(lookup) not_supported_rows{};

         partition_lookup(
            std::begin(lookup), 
            std::end(lookup),
            std::begin(mask), 
            std::end(mask),
            std::back_inserter(supported_rows),
            std::back_inserter(not_supported_rows));

         const auto supported_cols = row_to_col(std::begin(supported_rows), std::end(supported_rows));
         // const auto [supported_rows, supported_cols] = supported_indicies(sys);

         const auto K_without_supports = numeric::drop(numeric::drop(K, supported_rows), supported_cols);
         const auto K_strich = numeric::drop(numeric::drop(K, not_supported_rows), supported_cols);
         const auto F_without_supports = numeric::drop(F, supported_rows);

         // consider sanitiy checks:
         // F.size1() == K_without_supports.size1() == K_without_supports.size2()
         // F.size1() == K_strich.size2()
         // expects(std::size(lookup) == n * dim);

         using precision_t = typename System::precision_t;
         using allocator_t = Allocator;

         using namespace boost::numeric;
         using namespace viennacl::linalg;

         const auto verschiebungen = Solver::displacement(K_without_supports, F_without_supports);

         //! \todo fix Allocator template arugment
         boost::numeric::ublas::vector<precision_t> auflagerreaktionen(K_strich.size1());
         boost::numeric::ublas::axpy_prod(K_strich, verschiebungen, auflagerreaktionen, false);

         const auto n = std::distance(std::begin(sys.nodes), std::end(sys.nodes));
         constexpr auto dim = System::dimension_v;

         std::vector<precision_t> support_reactions(n * dim);
         std::vector<precision_t> displacements(n * dim);

         //! \todo could be only on zip() call -> benchmark
         zip(
            [&support_reactions](auto&& tuple) { support_reactions[static_cast<int>(std::get<row>(tuple))] = std::get<1>(tuple); },
            std::begin(supported_rows), 
            std::end(supported_rows), 
            std::begin(auflagerreaktionen));
         zip(
            [&displacements](auto&& tuple) { displacements[static_cast<int>(std::get<row>(tuple))] = std::get<1>(tuple); },
            std::begin(not_supported_rows), 
            std::end(not_supported_rows), 
            std::begin(verschiebungen));

         system_result<System> result;

         std::transform(        
            std::begin(sys.nodes), 
            std::end(sys.nodes),
            std::inserter(result.node, std::end(result.node)),
            [&result](auto&& node){ return std::make_pair(node.name, typename decltype(result)::node_result{}); });

         auto it = std::begin(result.node);
         for_each_chunk<dim>(
            std::begin(support_reactions), 
            std::end(support_reactions), 
            //! \todo is not working with msvc: [it = std::begin(result.node)](auto x, auto y) mutable 
            [&it](auto x, auto y)
            {
               using value_t = typename decltype(it->second.support_reaction)::value_type;
               it->second.support_reaction = { value_t{ x }, value_t{ y } };
               it++;
            });

         it = std::begin(result.node);
         for_each_chunk<dim>(
            std::begin(displacements), 
            std::end(displacements), 
            //! \todo is not working with msvc: [it = std::begin(result.node)](auto x, auto y) mutable 
            [&it](auto x, auto y)
            {
               using value_t = typename decltype(it->second.displacement)::value_type;
               it->second.displacement = { value_t{ x }, value_t{ y } };
               it++;
            });

         //! \todo way to complex and not parallelizable
         for (auto const& m : sys.members)
         {
            const auto[s, e] = sys.coincidence_table.at(m.name);

            auto delta_s = result.node[s].displacement;
            auto delta_e = result.node[e].displacement;

            const auto EA = m.E.value() * m.A.value();
            const auto kv = EA / length(sys, m.name).value();
            // const auto kv = EA_over_l(m.E, m.A, length(sys, m.id)).value();         

            const auto alpha = impl::angle_to_x_axis(sys, m.name);
            //! \todo k1?
            const auto k1 = alpha < 0.0 ? -1.0 : 1.0;

            const auto N = (
               -std::cos(alpha) * X(delta_s).value() +
               -std::sin(alpha) * Y(delta_s).value() +
                std::cos(alpha) * X(delta_e).value() +
                std::sin(alpha) * Y(delta_e).value()) * k1 * kv;

            result.members.insert({ m.name, N });
         }

         for (const auto [node, res] : result.node)
            result.nodes.insert({ node, res.displacement, res.support_reaction });

         if (size(result.members) != size(sys.members) || size(result.nodes) != size(sys.nodes))
            return std::tuple{ false, result };

         return std::tuple{ true, result };
      }
   }

   //! \todo solver template argument -> forward
   template <typename System>
   [[nodiscard]] auto solve(System const& sys, loadcase<System> const& load)
   {
      using precision_t = typename System::precision_t;
      using value_t = units::quantity<units::newton, precision_t>;

      constexpr auto dim = System::dimension_v;
#if 0
      std::array<std::byte, 2048> buffer; 
      std::pmr::monotonic_buffer_resource rsrc(buffer.data(), size(buffer));

      std::pmr::vector<value_t> action_on_structure(&rsrc);
      action_on_structure.resize(std::size(sys.nodes) * dim);

      std::array<std::byte, 4096> matrix_buffer;
      std::pmr::monotonic_buffer_resource rsrc_1(matrix_buffer.data(), size(matrix_buffer));
      std::pmr::set_default_resource(&rsrc_1);
#endif
      //! \todo remove or replace with pmr::
      using allocator_t = numeric::ublas_allocator_wrapper<std::allocator<double>>;
      //! \todo parallelize K and F
      auto K = impl::global_stiffness_matrix<allocator_t>(sys);

      std::vector<value_t> action_on_structure(size(sys.nodes) * dim);
      add_node_load(sys, load, action_on_structure);
      //! \todo add_node_load(sys, load, begin(action_on_structure));
      add_temperature_load(sys, load, begin(action_on_structure));

      // convert quantities to a more suitable datatype, in this case to ublas::vector<T>
      auto F = numeric::make_vector<allocator_t>(
         begin(action_on_structure), 
         end(action_on_structure), 
         [](auto&& q) -> typename System::precision_t { return q.value(); });

      //! \todo return std::optional or boost::outcome
      //! \todo process(solve) -> post_process(sys, result)
      return impl::solve<solver::backend::viennacl_impl>(sys, K, F);
   }   
}
