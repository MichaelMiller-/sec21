#pragma once

#include <sec21/expects.h>

#include <boost/qvm/vec_access.hpp>
#include <boost/qvm/vec_operations.hpp>

namespace sec21
{
   template <typename T>
   // requires T a < b;
   struct aabb
   {
      using value_t = T;
      value_t min{};
      value_t max{};

      //! \todo
      auto length_x() const {}

      template <typename U>
      void translate(U u) noexcept
      {
         min = min + u;
         max = max + u;
      }

      auto center() const noexcept
      {
         using namespace boost::qvm;
         const auto mid = (max - min) / 2;

         return mid;
      }

      template <typename U>
      friend auto operator * (aabb const& lhs, U const& rhs) noexcept
      {
         return aabb<decltype(lhs.min * rhs)>{lhs.min * rhs, lhs.max * rhs};
      }

      friend auto& operator << (std::ostream& os, aabb const& obj) noexcept {
         return os << "[ min: " << obj.min << " max: " << obj.max << "]";
      }
   };

   template <typename T>
   auto bounding_rect(std::vector<T> const& vertices) -> aabb<T>
   {
      using namespace boost::qvm;

      const auto it_max_x = std::max_element(begin(vertices), end(vertices), [](auto lhs, auto rhs){ return X(lhs) < X(rhs); });
      const auto it_max_y = std::max_element(begin(vertices), end(vertices), [](auto lhs, auto rhs){ return Y(lhs) < Y(rhs); });
      expects([=](){ return it_max_x != end(vertices); }, "expects a max element");
      expects([=](){ return it_max_y != end(vertices); }, "expects a max element");

      const auto it_min_x = std::min_element(begin(vertices), end(vertices), [](auto lhs, auto rhs){ return X(lhs) < X(rhs); });
      const auto it_min_y = std::min_element(begin(vertices), end(vertices), [](auto lhs, auto rhs){ return Y(lhs) < Y(rhs); });
      expects([=](){ return it_min_x != end(vertices); }, "expects a min element");
      expects([=](){ return it_min_y != end(vertices); }, "expects a min element");

      return {{X(*it_min_x), Y(*it_min_y)}, {X(*it_max_x), Y(*it_max_y)}};
   }
}