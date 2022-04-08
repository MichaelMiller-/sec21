#pragma once

#include <boost/qvm/vec_access.hpp>

#include <algorithm>
#include <cmath>
#include <vector>

namespace sec21
{
   template <typename Vertex>
   // requires typename Vertex::value_t;
   struct polygon
   {
      using value_t = Vertex;
      using underlaying_value_t = typename Vertex::value_t;

      std::vector<Vertex> vertices;

      template <typename U>
      void translate(U v) noexcept
      {
         std::transform(begin(vertices), end(vertices), begin(vertices),
                        [v](auto p) { return p + v; });
      }

      auto extent() const noexcept
      {
         using namespace boost::qvm;

         underlaying_value_t result{};
         const auto n = size(vertices);

         for (auto i = 0; i < n; ++i) {
            result += std::sqrt(std::pow(X(vertices[i + 1 < n ? i + 1 : 0]) - X(vertices[i]), 2) +
                                std::pow(Y(vertices[i + 1 < n ? i + 1 : 0]) - Y(vertices[i]), 2));
         }
         return result;
      }
      auto area() const noexcept
      {
         using namespace boost::qvm;

         underlaying_value_t result{};
         const auto n = size(vertices);

         for (auto i = 0; i < n; ++i) {
            result += (X(vertices[i]) * Y(vertices[i + 1 < n ? i + 1 : 0])) -
                      (Y(vertices[i]) * X(vertices[i + 1 < n ? i + 1 : 0]));
         }
         return std::abs(result) / 2;
      }
      auto center() const noexcept
      {
         using namespace boost::qvm;

         const auto n = size(vertices);
         const long double a = area();
         const long double k0 = 1 / (6 * a);

         long double xs{};
         long double ys{};

         for (auto i = 0; i < n; ++i) {
            xs +=
               (X(vertices[i]) + X(vertices[i + 1 < n ? i + 1 : 0])) *
               (X(vertices[i]) * Y(vertices[i + 1 < n ? i + 1 : 0]) - X(vertices[i + 1 < n ? i + 1 : 0]) * Y(vertices[i]));
            ys +=
               (Y(vertices[i]) + Y(vertices[i + 1 < n ? i + 1 : 0])) *
               (X(vertices[i]) * Y(vertices[i + 1 < n ? i + 1 : 0]) - X(vertices[i + 1 < n ? i + 1 : 0]) * Y(vertices[i]));
         }
         return Vertex{static_cast<underlaying_value_t>(xs * k0), static_cast<underlaying_value_t>(ys * k0)};
      }

      template <typename Shape>
      static auto from(Shape shape) -> polygon
      {
         return to_polygon<Vertex>(std::move(shape));
      }
   };

   template <typename Vertex>
   static auto to_triangle_list(polygon<Vertex> poly) -> std::vector<Vertex>
   {
      if (size(poly.vertices) < 3)
         throw std::invalid_argument{"to_triangle_list: too few vertices"};

      std::vector<Vertex> result;
      for (auto i = 0, c = 1; i < size(poly.vertices); i+=2, ++c)
      {
         result.push_back(poly.vertices[0]);
         result.push_back(poly.vertices[c]);
         result.push_back(poly.vertices[c+1]);
      }
      return result;
   }
} // namespace sec21

#include <nlohmann/json.hpp>

namespace sec21
{
   template <typename Vertex>
   void to_json(nlohmann::json& j, polygon<Vertex> const& obj)
   {
      j["vertices"] = obj.vertices;
   }

   template <typename Vertex>
   void from_json(nlohmann::json const& j, polygon<Vertex>& obj)
   {
      j.at("vertices").get_to(obj.vertices);
   }
} // namespace sec21
