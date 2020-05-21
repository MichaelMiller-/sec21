#pragma once

#include <sec21/units.h>
#include <sec21/strong_type.h>
#include <sec21/structural_analysis/support.h>
#include <sec21/structural_analysis/force.h>

#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/core/cs.hpp>

#include <type_traits>
#include <limits>
#include <optional>

namespace sec21::structural_analysis
{
   template <auto Dimension, typename Precision = double>
   struct node
   {
      static_assert(Dimension == 2, "Only works in 2D just yet");
      static_assert(std::is_floating_point<Precision>::value, "Precision is not a floating-point type");
      static_assert(std::numeric_limits<Precision>::is_iec559, "Floating-point type does not fulfill the requirements of IEC 559");

      constexpr static auto dimension_v = Dimension;

      using descriptor_t = size_t;
      using precision_t = Precision;
      //! \todo replace with boost::qvm
      using point_t = boost::geometry::model::point<precision_t, dimension_v, boost::geometry::cs::cartesian>;
      using load_t = force_t<dimension_v>;
      using global_support_t = support<dimension_v>;

      //! \todo 2019-04-23 use strong_type
      descriptor_t id{ std::numeric_limits<descriptor_t>::max() };
      //! \brief World Position
      point_t position{};

      std::optional<global_support_t> global_support{};
      //! \todo could be in a extra struct; like system_load aka node_load
      std::optional<load_t> load{};
   };

   template <typename T>
   struct is_node : std::false_type {};

   template <auto Dimension, typename Precision>
   struct is_node<node<Dimension, Precision>> : std::true_type {};

   template <typename T>
   constexpr bool is_node_v = is_node<T>::value;

   static_assert(is_node_v<node<2, float>>);
   static_assert(is_node_v<node<2, double>>);
   static_assert(is_node_v<int> == false);
   // static_assert(std::is_trivial_v<node<2, float>>, "node is not trivial");

   template <typename InputIterator, typename OutputIterator>
   auto support_mask(InputIterator first, InputIterator last, OutputIterator out) -> OutputIterator
   {
      // static_assert(std::is_same_v<typename std::iterator_traits<OutputIterator>::value_type, bool>);
      using value_t = typename std::iterator_traits<InputIterator>::value_type;

      while (first != last) 
      {
         if (first->global_support)
         {
            out = std::copy(
               std::begin(first->global_support.value()), 
               std::end(first->global_support.value()), 
               out);
         }
         else
         {
            for (auto i = 0; i < value_t::dimension_v; ++i)
               *out++ = false;
         }
         ++first;
      }
      return out;
   }
}

#include <nlohmann/json.hpp>

namespace nlohmann 
{
   template <typename T>
   struct adl_serializer<boost::geometry::model::point<T, 2, boost::geometry::cs::cartesian>> 
   {
      using type_t = boost::geometry::model::point<T, 2, boost::geometry::cs::cartesian>;

      static void to_json(json& j, type_t const& pt) 
      {
         j = json{
            { "x", boost::geometry::get<0>(pt) },
            { "y", boost::geometry::get<1>(pt) }
         };
      }
      static void from_json(json const& j, type_t& pt) 
      {
         T x;
         T y;
         j.at("x").get_to(x);
         j.at("y").get_to(y);
         boost::geometry::set<0>(pt, x);
         boost::geometry::set<1>(pt, y);
      }
   };

   template <typename T>
   struct adl_serializer<boost::geometry::model::point<T, 3, boost::geometry::cs::cartesian>> 
   {
      using type_t = boost::geometry::model::point<T, 3, boost::geometry::cs::cartesian>;

      static void to_json(json& j, type_t const& pt) 
      {
         j = json{
            { "x", boost::geometry::get<0>(pt) },
            { "y", boost::geometry::get<1>(pt) },
            { "z", boost::geometry::get<2>(pt) }
         };
      }
      static void from_json(json const& j, type_t& pt) 
      {
         T x;
         T y;
         T z;
         j.at("x").get_to(x);
         j.at("y").get_to(y);
         j.at("z").get_to(z);
         boost::geometry::set<0>(pt, x);
         boost::geometry::set<1>(pt, y);
         boost::geometry::set<2>(pt, z);
      }
   };

   template <typename T>
   struct adl_serializer<std::optional<T>> 
   {
      static void to_json(json& j, std::optional<T> const& opt) 
      {
         if (opt == std::nullopt) 
            j = nullptr;
         else
            j = *opt;
      }
      static void from_json(json const& j, std::optional<T>& opt) 
      {
         if (j.is_null()) 
            opt = std::nullopt;
         else
            opt = j.get<T>();
      }
   };
   template <>
   struct adl_serializer<sec21::structural_analysis::force_2D_t> 
   {
      using type_t = sec21::structural_analysis::force_2D_t;

      static void to_json(json& j, type_t const& t) 
      {
         j = json{
            {"x", std::get<0>(t)},
            {"y", std::get<1>(t)}
         };
      }
      static void from_json(json const& j, type_t& t) 
      {
         j.at("x").get_to(std::get<0>(t));
         j.at("y").get_to(std::get<1>(t));
      }
   };
   template <>
   struct adl_serializer<sec21::structural_analysis::support<2>> 
   {
      using type_t = sec21::structural_analysis::support<2>;

      static void to_json(json& j, type_t const& t) 
      {
         j = json{
            {"x", std::get<0>(t)},
            {"y", std::get<1>(t)}
         };
      }
      static void from_json(json const& j, type_t& t) 
      {
         j.at("x").get_to(std::get<0>(t));
         j.at("y").get_to(std::get<1>(t));
      }
   };      
}

namespace sec21::structural_analysis
{
   template <auto Dimension, typename Precision>
   void to_json(nlohmann::json& j, node<Dimension, Precision> const& obj) {
      j = nlohmann::json{
         {"id", obj.id},
         {"position", obj.position}, 
         {"global_support", obj.global_support},
         {"load", obj.load}
      };
   }
   template <auto Dimension, typename Precision>
   void from_json(nlohmann::json const& j, node<Dimension, Precision>& obj) 
   {
      j.at("id").get_to(obj.id);
      j.at("position").get_to(obj.position);
      j.at("global_support").get_to(obj.global_support);
      j.at("load").get_to(obj.load);
   }
}