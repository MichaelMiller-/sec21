#pragma once

#include <boost/math/constants/constants.hpp>

#include <sec21/structural_analysis/type_traits.h>
#include <sec21/structural_analysis/force.h>

#include <sec21/units/dimensions/force.h>
#include <sec21/units/quantity.h>

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
               std::transform(std::begin(first->load), std::end(first->load), out, [](auto v) { return T{ v.value() }; });
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
      using value_t = double; // units::quantity<units::newton, typename System::precision_t>;

      std::vector<value_t> result;
      //! \todo 2019-05-15 a member projection would be nice (node::load)
      detail::lastvektor_F_impl<value_t>::apply(std::begin(sys.nodes), std::end(sys.nodes), std::back_inserter(result));
      return result;
   }
}

namespace sec21::structural_analysis::impl
{
   //! \brief Knotenpunktslasten
   template <typename System>
   [[nodiscard]] auto load_vector_F(System const& sys) noexcept 
      -> std::enable_if_t<is_space_truss_2D<System>::value, std::vector<units::quantity<units::newton, typename System::precision_t>>>
   {
      using node_t = typename decltype(sys.nodes)::value_type;
      using load_t = typename node_t::load_t;
      using value_t = units::quantity<units::newton, typename System::precision_t>;
     
      std::vector<value_t> result{};

      //! \todo std::copy
      for (auto n :sys.nodes)
      {
         for (auto l : n.load)
         {
            result.push_back(l);
         }
      }
      return result;
   }
}