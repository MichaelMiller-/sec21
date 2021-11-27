#pragma once

#include <sec21/numeric/flatten.h>
#include <sec21/numeric/make_vector.h>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>

#include <Eigen/Dense>

#include <vector>

namespace sec21::structural_analysis::solver::backend
{
   struct eigen
   {
      template <typename Precision>
      [[nodiscard]] static auto displacement(
         Eigen::Matrix<Precision, Eigen::Dynamic, Eigen::Dynamic> const& K,
         Eigen::Matrix<Precision, Eigen::Dynamic, 1> const& F) -> Eigen::Matrix<Precision, Eigen::Dynamic, 1>
      {
         if (std::size(K) != std::pow(std::size(F), 2))
            throw std::invalid_argument("size mismatch");

         const auto lu = Eigen::FullPivLU<Eigen::Matrix<Precision, -1, -1>>(K);
         return lu.solve(F);
      }

      template <typename Precision, typename Allocator>
      [[nodiscard]] static auto displacement(
         std::vector<Precision, Allocator> const& K,
         std::vector<Precision, Allocator> const& F) -> std::vector<Precision, Allocator>
      {
         if (std::size(K) != std::pow(std::size(F), 2))
            throw std::invalid_argument("size mismatch");

         //! \todo consider alignment
         Eigen::Matrix<Precision, Eigen::Dynamic, Eigen::Dynamic> kk = Eigen::Map<const Eigen::Matrix<Precision, -1, -1>>(K.data(), F.size(), F.size());
         Eigen::Matrix<Precision, Eigen::Dynamic, 1> ff = Eigen::Map<const Eigen::Matrix<Precision, -1, 1>, Eigen::Unaligned>(F.data(), F.size());

         const auto result = displacement(kk, ff);

         return { result.data(), result.data() + result.cols() * result.rows() };
      }

      template <typename Precision, typename Allocator>
      [[nodiscard]] static auto displacement(
         boost::numeric::ublas::matrix<Precision, boost::numeric::ublas::row_major, Allocator> K,
         boost::numeric::ublas::vector<Precision, Allocator> F) -> boost::numeric::ublas::vector<Precision, Allocator>
      {
         const auto ff = std::vector<Precision>{F.begin(), F.end()};
         const auto result = displacement(numeric::flatten(K), ff);
         using T = typename decltype(result)::allocator_type;
         return numeric::make_vector<T>(std::begin(result), std::end(result));
      }      
   };
}