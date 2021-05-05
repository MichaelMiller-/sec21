#pragma once

// disable non MPL2 compatible features, or in other words disable the features which are still under the LGPL.
// #define EIGEN_MPL2_ONLY
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
            throw std::invalid_argument("size did not match");

         const auto lu = Eigen::FullPivLU<Eigen::MatrixXd>(K);
         return lu.solve(F);
      }

      template <typename Precision, typename Allocator>
      [[nodiscard]] static auto displacement(
         std::vector<Precision, Allocator> const& K,
         std::vector<Precision, Allocator> const& F) -> std::vector<Precision, Allocator>
      {
         if (std::size(K) != std::pow(std::size(F), 2))
            throw std::invalid_argument("size did not match");

            //! \todo consider alignment
         const Eigen::MatrixXd kk = Eigen::Map<const Eigen::MatrixXd>(K.data(), F.size(), F.size());
         const Eigen::VectorXd ff = Eigen::Map<const Eigen::VectorXd, Eigen::Unaligned>(F.data(), F.size());

         const auto result = displacement(kk, ff);

         return{ result.data(), result.data() + result.cols() * result.rows() };
      }
   };
}