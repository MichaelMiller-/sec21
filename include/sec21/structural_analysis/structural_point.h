#pragma once

#include <sec21/structural_analysis/descriptor_traits.h>

#include <boost/uuid/uuid.hpp>

#include <string>

namespace sec21::structural_analysis
{
   struct structural_point
   {
      using precision_t = double; //! \todo
      using descriptor_t = std::size_t;

      //! \todo remove
      static constexpr auto dimension_v = 2;

      static_assert(std::is_floating_point_v<precision_t>);
      static_assert(is_descriptor_v<descriptor_t>);

      descriptor_t id{descriptor_traits<descriptor_t>::invalid()};

      std::string name{};

      boost::uuids::uuid tag{};

      //! \todo unit [m]
      precision_t coordinate_x{};
      precision_t coordinate_y{};
      precision_t coordinate_z{};

      friend auto& operator<<(std::ostream& os, structural_point const& obj)
      {
         return os << obj.name << " (" << obj.coordinate_x << "," << obj.coordinate_y << "," << obj.coordinate_z << ")";
      }
   };
} // namespace sec21::structural_analysis

#include <boost/qvm/vec_traits.hpp>
#include <boost/qvm/vec_traits_defaults.hpp>

namespace boost::qvm
{
   template <>
   struct vec_traits<sec21::structural_analysis::structural_point>
       : vec_traits_defaults<sec21::structural_analysis::structural_point,
                             sec21::structural_analysis::structural_point::precision_t, 3>
   {
      template <auto I>
      static inline scalar_type& write_element(sec21::structural_analysis::structural_point& v)
      {
         static_assert(I < 3);
         if constexpr (I == 0)
            return v.coordinate_x;
         if constexpr (I == 1)
            return v.coordinate_y;
         if constexpr (I == 2)
            return v.coordinate_z;
      }
   };
} // namespace boost::qvm
