#pragma once

#include <sec21/array_qvm_vec_traits.h>
#include <sec21/units/dimensions/force.h>
#include <sec21/units/quantity.h>

#include <boost/qvm/vec_access.hpp>

#include <array>
#include <type_traits>

namespace sec21::structural_analysis
{
   //! \todo what about std::valarray?
   template <auto Dimension> //, typename Precision = double>
   using force_t = std::array<units::quantity<units::kilonewton, double>, Dimension>;

   using force_2D_t = force_t<2>;
   using force_3D_t = force_t<3>;

   template <auto Dimension> //, typename Precision>
   [[nodiscard]] constexpr auto operator+(force_t<Dimension> const& lhs, force_t<Dimension> const& rhs) noexcept
      -> force_t<Dimension>
   {
      using namespace boost::qvm;
      if constexpr (Dimension == 2) {
         return {{X(lhs) + X(rhs), Y(lhs) + Y(rhs)}};
      }
      if constexpr (Dimension == 3) {
         return {{X(lhs) + X(rhs), Y(lhs) + Y(rhs), Z(lhs) + Z(rhs)}};
      }
   }

   template <typename Iterator>
   [[nodiscard]] auto superposition(Iterator first, Iterator last) noexcept
   {
      typename std::iterator_traits<Iterator>::value_type result{};

      for (auto it = first; it != last; ++it)
         result = result + *it;
      //! \todo 2019-08-08: is not working: return std::reduce(first, last);
      return result;
   }
} // namespace sec21::structural_analysis

#include <nlohmann/json.hpp>

namespace nlohmann
{
   template <>
   struct adl_serializer<sec21::structural_analysis::force_2D_t>
   {
      using type_t = sec21::structural_analysis::force_2D_t;

      static void to_json(json& j, type_t const& t) { j = json{{"x", std::get<0>(t)}, {"y", std::get<1>(t)}}; }
      static void from_json(json const& j, type_t& t)
      {
         j.at("x").get_to(std::get<0>(t));
         j.at("y").get_to(std::get<1>(t));
      }
   };
} // namespace nlohmann