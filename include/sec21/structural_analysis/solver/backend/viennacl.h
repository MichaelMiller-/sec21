#pragma once

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>

#ifdef __clang__

#else
// #pragma GCC diagnostic push
// #pragma GCC diagnostic ignored "-Wignored-attributes"

// #pragma GCC diagnostic pop
#endif
#include <viennacl/linalg/lu.hpp>
#include <viennacl/matrix.hpp>
#include <viennacl/vector.hpp>

namespace sec21::structural_analysis::solver::backend
{
   //! name clashing with namespace "viennacl".
   struct viennacl_impl
   {
      template <typename Precision, typename Allocator>
      [[nodiscard]] static auto
      displacement(boost::numeric::ublas::matrix<Precision, boost::numeric::ublas::row_major, Allocator> K,
                   boost::numeric::ublas::vector<Precision, Allocator> F)
         -> boost::numeric::ublas::vector<Precision, Allocator>
      {
         using namespace viennacl::linalg;
         using precision_t = Precision;

         viennacl::matrix<precision_t> vcl_matrix(K.size1(), K.size2());
         viennacl::vector<precision_t> vcl_rhs(F.size());
         viennacl::copy(K, vcl_matrix);
         viennacl::copy(F, vcl_rhs);

         solve(vcl_matrix, vcl_rhs, viennacl::linalg::upper_tag{});
         solve(vcl_matrix, vcl_rhs, viennacl::linalg::lower_tag{});
         lu_factorize(vcl_matrix);
         lu_substitute(vcl_matrix, vcl_rhs);

         boost::numeric::ublas::vector<precision_t, Allocator> result(K.size1());
         viennacl::copy(std::begin(vcl_rhs), std::end(vcl_rhs), std::begin(result));

         return result;
      }

      template <typename Precision, typename Allocator>
      [[nodiscard]] static auto support_reactions(
         boost::numeric::ublas::matrix<Precision, boost::numeric::ublas::row_major, Allocator> K,
         boost::numeric::ublas::vector<Precision, Allocator> displacements) //! \todo -> std::vector<Precision>
      {
         using namespace viennacl::linalg;
         using precision_t = Precision;

         viennacl::matrix<precision_t> vcl_A(K.size1(), K.size2());
         viennacl::vector<precision_t> vcl_b(displacements.size());

         viennacl::copy(K, vcl_A);
         viennacl::copy(displacements, vcl_b);

         viennacl::vector<precision_t> vcl_result(K.size1());
         vcl_result = viennacl::linalg::prod(vcl_A, vcl_b);

         //! \todo fix allocator template argument
         boost::numeric::ublas::vector<precision_t> result(K.size1());
         viennacl::copy(std::begin(vcl_result), std::end(vcl_result), std::begin(result));

         return result;
      }
   };
} // namespace sec21::structural_analysis::solver::backend