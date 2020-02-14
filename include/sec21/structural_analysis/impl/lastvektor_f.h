#pragma once

#include <boost/math/constants/constants.hpp>

namespace sec21::structural_analysis::impl
{
   namespace detail 
   {
      template <typename T>
      struct lastvektor_F_impl
      {
         template <typename InputIterator, typename OutputIterator>
         static void apply(InputIterator first, InputIterator last, OutputIterator out) noexcept
         {
            for (; first != last; ++first)
               std::transform(std::begin(first->load), std::end(first->load), out, [](auto v) { return v.value(); });
         }
      };
   }   
   //! \todo 2019-05-15 return type; should something like Netwon-Value (kN)
   //! \todo 2019-08-27 maybe std::valarray instead of std::vector
   template <typename System>
   [[nodiscard]] auto lastvektor_F(System const& sys) noexcept -> std::vector<double>
   {
      constexpr auto dim = System::node_t::dimension_v;
      static_assert(dim == 2, "currently only works with 2D systems");

      using node_t = typename decltype(sys.nodes)::value_type;
      using load_t = decltype(node_t::load);
      using result_t = double;

      std::vector<result_t> result;
      //! \todo 2019-05-15 a member projection would be nice (node::load)
      detail::lastvektor_F_impl<result_t>::apply(std::begin(sys.nodes), std::end(sys.nodes), std::back_inserter(result));
      return result;
   }
}