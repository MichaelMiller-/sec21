#pragma once

namespace sec21::structural_analysis::shape
{
   template <typename T>
   struct cross_section_vertex
   {
      using value_t = T;

      T y{}; // [mm]
      T z{}; // [mm]
#ifdef __cpp_lib_three_way_comparison
      constexpr bool operator==(cross_section_vertex const&) const = default;
#else
      constexpr bool operator==(cross_section_vertex const& obj) const
      {
         return std::tie(y, z) == std::tie(obj.y, obj.z);
      }
#endif
      template <typename U>
      friend auto operator*(cross_section_vertex const& lhs, U const& rhs) noexcept
      {
         return cross_section_vertex{lhs.y * rhs, lhs.z * rhs};
      }
      friend auto operator+(cross_section_vertex const& lhs, cross_section_vertex const& rhs) noexcept
      {
         return cross_section_vertex{lhs.y + rhs.y, lhs.z + rhs.z};
      }
      friend auto operator<(cross_section_vertex const& lhs, cross_section_vertex const& rhs) noexcept
      {
         return lhs.y < rhs.y || lhs.z < rhs.z;
      }
      friend auto& operator<<(std::ostream& os, cross_section_vertex const& obj)
      {
         return os << "(" << obj.y << "," << obj.z << ")";
      }
   };
} // namespace sec21::structural_analysis::shape

#include <boost/qvm/vec_traits_defaults.hpp>

namespace boost::qvm
{
   template <typename T>
   struct vec_traits<sec21::structural_analysis::shape::cross_section_vertex<T>> : vec_traits_defaults<sec21::structural_analysis::shape::cross_section_vertex<T>, T, 2>
   {
      using scalar_type = T;

      template <auto I>
      static inline scalar_type& write_element(sec21::structural_analysis::shape::cross_section_vertex<T>& v)
      {
         if constexpr (I == 0)
            return v.y;
         if constexpr (I == 1)
            return v.z;
      }
   };
} // namespace boost::qvm

#include <nlohmann/json.hpp>

namespace sec21::structural_analysis::shape
{
   template <typename T>
   void to_json(nlohmann::json& j, cross_section_vertex<T> const& obj)
   {
      j["y"] = obj.y;
      j["z"] = obj.z;
   }

   template <typename T>
   void from_json(nlohmann::json const& j, cross_section_vertex<T>& obj)
   {
      j.at("y").get_to(obj.y);
      j.at("z").get_to(obj.z);
   }
} // namespace sec21::structural_analysis::shape
