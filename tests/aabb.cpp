#include <catch.hpp>

#include <sec21/aabb.h>

struct point2d
{
   double x{};
   double y{};

   friend bool operator==(point2d const& lhs, point2d const& rhs)
   {
      return lhs.x == Catch::Approx(rhs.x) && lhs.y == Catch::Approx(rhs.y);
   }
   friend auto& operator<<(std::ostream& os, point2d const& obj) { return os << "(" << obj.x << "," << obj.y << ")"; }
};

namespace boost::qvm
{
   template <>
   struct vec_traits<point2d>
   {
      static int const dim = 2;
      using scalar_type = double;

      template <int I>
      static inline auto& write_element(point2d& v)
      {
         if constexpr (I == 0) {
            return v.x;
         }
         if constexpr (I == 1) {
            return v.y;
         }
      }
      template <int I>
      static inline auto read_element(point2d const& v)
      {
         if constexpr (I == 0) {
            return v.x;
         }
         if constexpr (I == 1) {
            return v.y;
         }
      }
   };
} // namespace boost::qvm

TEST_CASE("2D axis aligned bounding box class", "[sec21]")
{
   using namespace sec21;
   using namespace boost::qvm;

   SECTION("calculate from vertices")
   {
      const auto input = std::vector<point2d>{
         {0, 0},
         {10, 10},
      };
      const auto result = bounding_rect(input);
      REQUIRE(result.center() == point2d{5, 5});
      REQUIRE(result.min() == point2d{0, 0});
      REQUIRE(result.max() == point2d{10, 10});
   }
   SECTION("calculate from triangle vertices")
   {
      const auto input = std::vector<point2d>{
         {0, 0},
         {20, 00},
         {10, 10},
      };
      const auto result = bounding_rect(input);
      REQUIRE(result.center() == point2d{10, 5});
      REQUIRE(result.min() == point2d{0, 0});
      REQUIRE(result.max() == point2d{20, 10});
   }
   SECTION("calculate from a negative moved triangle")
   {
      const auto input = std::vector<point2d>{
         {-10, -50},
         {20, -70},
         {10, 10},
      };
      const auto result = bounding_rect(input);
      REQUIRE(result.center() == point2d{5, -30});
      REQUIRE(result.min() == point2d{-10, -70});
      REQUIRE(result.max() == point2d{20, 10});
   }
   SECTION("translate")
   {
      auto result = bounding_rect<point2d>({
         {0, 0},
         {10, 20},
      });
      result.translate(point2d{5, 5});
      REQUIRE(result.center() == point2d{10, 15});
      REQUIRE(result.min() == point2d{5, 5});
      REQUIRE(result.max() == point2d{15, 25});
   }
}

struct point3d
{
   double x{};
   double y{};
   double z{};

   friend bool operator==(point3d const& lhs, point3d const& rhs)
   {
      return lhs.x == Catch::Approx(rhs.x) && lhs.y == Catch::Approx(rhs.y) && lhs.z == Catch::Approx(rhs.z);
   }
   friend auto& operator<<(std::ostream& os, point3d const& obj)
   {
      return os << "(" << obj.x << "," << obj.y << "," << obj.z << ")";
   }
};

namespace boost::qvm
{
   template <>
   struct vec_traits<point3d>
   {
      static int const dim = 3;
      using scalar_type = double;

      template <int I>
      static inline auto& write_element(point3d& v)
      {
         if constexpr (I == 0) {
            return v.x;
         }
         if constexpr (I == 1) {
            return v.y;
         }
         if constexpr (I == 2) {
            return v.z;
         }
      }
      template <int I>
      static inline auto read_element(point3d const& v)
      {
         if constexpr (I == 0) {
            return v.x;
         }
         if constexpr (I == 1) {
            return v.y;
         }
         if constexpr (I == 2) {
            return v.z;
         }
      }
   };
} // namespace boost::qvm

TEST_CASE("3D axis aligned bounding box class", "[sec21]")
{
   using namespace sec21;
   using namespace boost::qvm;

   SECTION("queries")
   {
      const auto result = aabb<point3d>{{0, 0, 0}, {10, 10, 10}};

      REQUIRE(result.center() == point3d{5, 5, 5});
      REQUIRE(result.min() == point3d{0, 0, 0});
      REQUIRE(result.max() == point3d{10, 10, 10});
   }
   SECTION("translate")
   {
      auto result = aabb<point3d>{{0, 0, 0}, {10, 10, 10}};
      result.translate(point3d{5, 5, 5});

      REQUIRE(result.center() == point3d{10, 10, 10});
      REQUIRE(result.min() == point3d{5, 5, 5});
      REQUIRE(result.max() == point3d{15, 15, 15});
   }
}
