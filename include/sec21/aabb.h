#pragma once

#include <sec21/expects.h>

#include <boost/qvm/vec_access.hpp>
#include <boost/qvm/vec_operations.hpp>

namespace sec21
{
   //! \brief axis aligned bounding box
   template <typename Point>
   class aabb
   {
      using value_t = Point;
      value_t minimum_coordinate{};
      value_t maximum_coordinate{};

    public:
      aabb(Point min, Point max) : minimum_coordinate{std::move(min)}, maximum_coordinate{std::move(max)} {}

      template <typename Vector>
      void translate(Vector const& delta) noexcept
      {
         using namespace boost::qvm;
         minimum_coordinate += delta;
         maximum_coordinate += delta;
      }

      auto center() const noexcept
      {
         using namespace boost::qvm;
         return (minimum_coordinate + maximum_coordinate) / 2;
      }

      auto min() const noexcept { return minimum_coordinate; }

      auto max() const noexcept { return maximum_coordinate; }

      friend auto& operator<<(std::ostream& os, aabb const& obj) noexcept
      {
         return os << "[ min: " << obj.minimum_coordinate << " max: " << obj.maximum_coordinate << "]";
      }
   };

   template <typename Point>
   auto bounding_rect(std::vector<Point> const& points) -> aabb<Point>
   {
      static_assert(boost::qvm::vec_traits<Point>::dim == 2, "only 2d vectors are allowed");

      using namespace boost::qvm;

      const auto it_max_x =
         std::max_element(begin(points), end(points), [](auto lhs, auto rhs) { return X(lhs) < X(rhs); });
      const auto it_max_y =
         std::max_element(begin(points), end(points), [](auto lhs, auto rhs) { return Y(lhs) < Y(rhs); });
      expects([=]() { return it_max_x != end(points); }, "expects a max element");
      expects([=]() { return it_max_y != end(points); }, "expects a max element");

      const auto it_min_x =
         std::min_element(begin(points), end(points), [](auto lhs, auto rhs) { return X(lhs) < X(rhs); });
      const auto it_min_y =
         std::min_element(begin(points), end(points), [](auto lhs, auto rhs) { return Y(lhs) < Y(rhs); });
      expects([=]() { return it_min_x != end(points); }, "expects a min element");
      expects([=]() { return it_min_y != end(points); }, "expects a min element");

      return {{X(*it_min_x), Y(*it_min_y)}, {X(*it_max_x), Y(*it_max_y)}};
   }

} // namespace sec21
