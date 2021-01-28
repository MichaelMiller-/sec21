#pragma once

#include <sec21/strong_type.h>
#include <sec21/structural_analysis/descriptor_traits.h>
#include <sec21/structural_analysis/support.h>

#include <type_traits>
#include <array>
#include <limits>
#include <optional>

namespace sec21::structural_analysis
{
   template <auto Dimension, typename Descriptor = int, typename Precision = double>
   struct node
   {
      static_assert(Dimension == 2, "Only works in 2D just yet");
      static_assert(std::is_floating_point<Precision>::value, "Precision is not a floating-point type");
      static_assert(std::numeric_limits<Precision>::is_iec559, "Floating-point type does not fulfill the requirements of IEC 559");

      constexpr static auto dimension_v = Dimension;

      using descriptor_t = Descriptor;
      using precision_t = Precision;

      using point_t = std::array<precision_t, dimension_v>;
      using global_support_t = support<dimension_v>;

      //! \brief unique name
      descriptor_t name{ descriptor_traits<descriptor_t>::invalid() };
      //! \brief World Position
      point_t position{};

      std::optional<global_support_t> global_support{};
   };

   //! \brief generates a list of supports per node and direction 
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
   //! \todo move to extra header
   template <typename T>
   struct adl_serializer<std::array<T, 2>> 
   {
      using type_t = std::array<T, 2>;

      static void to_json(json& j, type_t const& pt)
      {
         j = json{
            { "x", std::get<0>(pt) },
            { "y", std::get<1>(pt) }
         };
      }
      static void from_json(json const& j, type_t& pt)
      {
         j.at("x").get_to(std::get<0>(pt));
         j.at("y").get_to(std::get<1>(pt));
      }
   };

   //! \todo move to extra header
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
   template <auto Dimension, typename Descriptor, typename Precision>
   void to_json(nlohmann::json& j, node<Dimension, Descriptor, Precision> const& obj) {
      j = nlohmann::json{
         {"name", obj.name},
         {"position", obj.position}, 
         {"global_support", obj.global_support}
      };
   }
   template <auto Dimension, typename Descriptor, typename Precision>
   void from_json(nlohmann::json const& j, node<Dimension, Descriptor, Precision>& obj) 
   {
      j.at("name").get_to(obj.name);
      j.at("position").get_to(obj.position);
      j.at("global_support").get_to(obj.global_support);
   }
}