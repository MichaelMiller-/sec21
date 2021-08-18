#pragma once

#include <sec21/erase_if_index.h>
#include <sec21/for_each_chunk.h>
#include <sec21/numeric/drop.h>
#include <sec21/numeric/make_vector.h>
#include <sec21/numeric/ublas_allocator_wrapper.h>
#include <sec21/row_col.h>
#include <sec21/structural_analysis/common.h>
#include <sec21/structural_analysis/impl/geometry.h>
#include <sec21/structural_analysis/impl/global_stiffness_matrix.h>
#include <sec21/structural_analysis/impl/lookup.h>
#include <sec21/structural_analysis/loadcase.h>
#include <sec21/structural_analysis/system_result.h>
#include <sec21/units.h>
#include <sec21/units/quantity.h>
#include <sec21/zip.h>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
//! \todo consider: <boost/numeric/ublas/opencl/operation.hpp>
#include <boost/numeric/ublas/operation.hpp>

#include <array>
#include <memory_resource>
#include <string_view>
#include <vector>

namespace sec21::structural_analysis
{
   namespace impl
   {
      template <typename Solver, typename Precision, typename Allocator, typename ProcessResult>
      [[nodiscard]] auto solve(std::size_t num_nodes,
                               boost::numeric::ublas::matrix<Precision, boost::numeric::ublas::row_major, Allocator> K,
                               boost::numeric::ublas::vector<Precision, Allocator> F, std::vector<row> lookup,
                               std::vector<bool> support_mask, ProcessResult&& process_result)
      {
         if (size(lookup) != size(support_mask))
            throw std::runtime_error("solve: mismatch sizes");

         if (K.size1() != F.size())
            throw std::runtime_error("solve: K + F size mismatch");

         std::decay_t<decltype(lookup)> supported_rows{};
         std::decay_t<decltype(lookup)> not_supported_rows{};

         partition_lookup(begin(lookup), end(lookup), begin(support_mask), end(support_mask),
                          std::back_inserter(supported_rows), std::back_inserter(not_supported_rows));

         const auto supported_cols = row_to_col(begin(supported_rows), end(supported_rows));

         //! \todo use std::vector
         const auto K_without_supports = numeric::drop(numeric::drop(K, supported_rows), supported_cols);
         const auto K_strich = numeric::drop(numeric::drop(K, not_supported_rows), supported_cols);
         const auto F_without_supports = numeric::drop(F, supported_rows);

         // consider sanity checks:
         // F.size1() == K_without_supports.size1() == K_without_supports.size2()
         // F.size1() == K_strich.size2()
         // expects(std::size(lookup) == n * dim);

         const auto verschiebungen = Solver::displacement(K_without_supports, F_without_supports);

         using precision_t = Precision; // typename System::precision_t;
         boost::numeric::ublas::vector<precision_t> auflagerreaktionen(K_strich.size1());
         boost::numeric::ublas::axpy_prod(K_strich, verschiebungen, auflagerreaktionen, false);

         constexpr auto dim = 2; // System::dimension_v;
         std::vector<precision_t> support_reactions(num_nodes * dim, 0.0);
         std::vector<precision_t> displacements(num_nodes * dim, 0.0);

         zip([&support_reactions](
                auto&& tuple) { support_reactions[static_cast<size_t>(std::get<row>(tuple))] = std::get<1>(tuple); },
             std::begin(supported_rows), std::end(supported_rows), std::begin(auflagerreaktionen));
         zip([&displacements](
                auto&& tuple) { displacements[static_cast<size_t>(std::get<row>(tuple))] = std::get<1>(tuple); },
             std::begin(not_supported_rows), std::end(not_supported_rows), std::begin(verschiebungen));

         return std::invoke(process_result, displacements, support_reactions);
      }

      template <typename Solver, typename System, typename Allocator>
      [[nodiscard]] auto
      solve(System const& sys, std::size_t num_nodes,
            boost::numeric::ublas::matrix<typename System::precision_t, boost::numeric::ublas::row_major, Allocator> K,
            boost::numeric::ublas::vector<typename System::precision_t, Allocator> F, std::vector<row> lookup,
            std::vector<bool> support_mask)
      {
         if (size(lookup) != size(support_mask))
            throw std::runtime_error("solve: mismatch sizes");

         if (K.size1() != F.size())
            throw std::runtime_error("solve: K + F size mismatch");

         std::decay_t<decltype(lookup)> supported_rows{};
         std::decay_t<decltype(lookup)> not_supported_rows{};

         partition_lookup(begin(lookup), end(lookup), begin(support_mask), end(support_mask),
                          std::back_inserter(supported_rows), std::back_inserter(not_supported_rows));

         const auto supported_cols = row_to_col(begin(supported_rows), end(supported_rows));

         //! \todo use std::vector
         const auto K_without_supports = numeric::drop(numeric::drop(K, supported_rows), supported_cols);
         const auto K_strich = numeric::drop(numeric::drop(K, not_supported_rows), supported_cols);
         const auto F_without_supports = numeric::drop(F, supported_rows);

         // consider sanity checks:
         // F.size1() == K_without_supports.size1() == K_without_supports.size2()
         // F.size1() == K_strich.size2()
         // expects(std::size(lookup) == n * dim);

         const auto verschiebungen = Solver::displacement(K_without_supports, F_without_supports);

         using precision_t = typename System::precision_t;
         boost::numeric::ublas::vector<precision_t> auflagerreaktionen(K_strich.size1());
         boost::numeric::ublas::axpy_prod(K_strich, verschiebungen, auflagerreaktionen, false);

         constexpr auto dim = System::dimension_v;
         std::vector<precision_t> support_reactions(num_nodes * dim, 0.0); //! \todo not saved in result!
         std::vector<precision_t> displacements(num_nodes * dim, 0.0);

         zip(
            [&support_reactions](auto&& tuple) {
               support_reactions[static_cast<std::size_t>(std::get<row>(tuple))] = std::get<1>(tuple);
            },
            std::begin(supported_rows), std::end(supported_rows), std::begin(auflagerreaktionen));
         zip([&displacements](
                auto&& tuple) { displacements[static_cast<std::size_t>(std::get<row>(tuple))] = std::get<1>(tuple); },
             std::begin(not_supported_rows), std::end(not_supported_rows), std::begin(verschiebungen));

         return make_result(sys, displacements, support_reactions);
      }

      template <typename Solver, typename System, typename Allocator>
      [[nodiscard]] auto
      solve(System const& sys,
            boost::numeric::ublas::matrix<typename System::precision_t, boost::numeric::ublas::row_major, Allocator> K,
            boost::numeric::ublas::vector<typename System::precision_t, Allocator> F)
      {
         std::vector<bool> mask{};
         support_mask(begin(sys.nodes), end(sys.nodes), std::back_inserter(mask));

         const auto lookup = impl::make_lookup(sys, row{});
         const auto num_nodes = std::distance(begin(sys.nodes), end(sys.nodes));

         return solve<Solver>(sys, num_nodes, K, F, lookup, mask);
      }

   } // namespace impl

   template <typename Solver, typename System>
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

      std::vector<value_t> action_on_structure(std::size(sys.nodes) * dim);
      add_node_load(sys, load, action_on_structure);
      //! \todo add_node_load(sys, load, begin(action_on_structure));
      add_temperature_load(sys, load, begin(action_on_structure));

      // convert quantities to a more suitable datatype, in this case to ublas::vector<T>
      auto F = numeric::make_vector<allocator_t>(begin(action_on_structure), end(action_on_structure),
                                                 [](auto&& q) -> typename System::precision_t { return q.value(); });

      //! \todo return std::optional or boost::outcome
      //! \todo process(solve) -> post_process(sys, result)
      return impl::solve<Solver>(sys, K, F);
   }
} // namespace sec21::structural_analysis
