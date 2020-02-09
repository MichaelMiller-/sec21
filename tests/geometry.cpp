#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <boost/geometry.hpp>

#include <boost/math/constants/constants.hpp>

#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/views/identity_view.hpp> 
#include <boost/geometry/geometries/register/point.hpp>
#include <boost/geometry/geometries/adapted/std_array.hpp>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>               // vec4, bvec4, dvec4, ivec4 and uvec4
#include <glm/mat2x2.hpp>             // mat2, dmat2
#include <glm/mat2x3.hpp>             // mat2x3, dmat2x3
#include <glm/mat2x4.hpp>             // mat2x4, dmat2x4
#include <glm/mat3x2.hpp>             // mat3x2, dmat3x2
#include <glm/mat3x3.hpp>             // mat3, dmat3
#include <glm/mat3x4.hpp>             // mat3x4, dmat2
#include <glm/mat4x2.hpp>             // mat4x2, dmat4x2
#include <glm/mat4x3.hpp>             // mat4x3, dmat4x3
#include <glm/mat4x4.hpp>             // mat4, dmat4
#include <glm/common.hpp>             // all the GLSL common functions: abs, min, mix, isnan, fma, etc.
#include <glm/exponential.hpp>        // all the GLSL exponential functions: pow, log, exp2, sqrt, etc.
// #include <glm/geometry.hpp>           // all the GLSL geometry functions: dot, cross, reflect, etc.
#include <glm/integer.hpp>            // all the GLSL integer functions: findMSB, bitfieldExtract, etc.
#include <glm/matrix.hpp>             // all the GLSL matrix functions: transpose, inverse, etc.
#include <glm/packing.hpp>            // all the GLSL packing functions: packUnorm4x8, unpackHalf2x16, etc.
#include <glm/trigonometric.hpp>      // all the GLSL trigonometric functions: radians, cos, asin, etc.
#include <glm/vector_relational.hpp>  // all the GLSL vector relational functions: equal, less, etc.

#include <iostream>

namespace sec21
{
   struct user_point
   {
      double x;
      double y;
      user_point(double x, double y) : x(x), y(y) {}
   };

   struct not_registered_point_type
   {
      double x;
      double y;
   };
}

// Customisation points - register all kind of point classes (third party, legacay, own types, ...)
//
BOOST_GEOMETRY_REGISTER_STD_ARRAY_CS(boost::geometry::cs::cartesian);
//
BOOST_GEOMETRY_REGISTER_POINT_2D(glm::vec2, float, boost::geometry::cs::cartesian, x, y)
BOOST_GEOMETRY_REGISTER_POINT_2D(sec21::user_point, double, boost::geometry::cs::cartesian, x, y)
//
BOOST_GEOMETRY_REGISTER_POINT_3D(glm::vec3, float, boost::geometry::cs::cartesian, x, y, z)

namespace sec21
{
   template <typename T>
   struct is_registered_point_type : std::is_same<typename ::boost::geometry::traits::tag<T>::type, ::boost::geometry::point_tag> {};

   template <typename T>
   inline constexpr bool is_registered_point_type_v = is_registered_point_type<T>::value;
}

static_assert(sec21::is_registered_point_type_v<sec21::not_registered_point_type> == false);
static_assert(sec21::is_registered_point_type_v<sec21::user_point>);
static_assert(sec21::is_registered_point_type_v<glm::vec2>, "glm::vec2 should be registered");
static_assert(sec21::is_registered_point_type_v<glm::vec3>, "glm::vec3 should be registered");

namespace sec21
{
   namespace detail
   {
      template<typename Target, typename T, std::size_t... Is>
      auto downcast_impl(T&& t, std::index_sequence<Is...>) noexcept -> Target
      {
         return { boost::geometry::get<Is>(std::forward<T>(t))... };
      }
   }
   
   template <typename Target, typename U>
   auto downcast(U const& point) noexcept -> Target
   {
      return detail::downcast_impl<Target>(point, std::make_index_sequence<::boost::geometry::dimension<U>::value>{});
   }

}
#pragma warning(push)
#pragma warning(disable:4201)
#include <glm/ext/matrix_transform.hpp> // perspective, translate, rotate
#include <glm/gtx/string_cast.hpp>
#pragma warning(pop)

//! \todo enable_if glm:: type
#if 0
template<typename T>
std::ostream& operator<<(std::ostream& os, T const& t)
{
   return os << glm::to_string(t);
}
#endif

namespace sec21
{
   template <typename Vector, typename Angle>
   struct transformation
   {
      using vector_t = Vector;
      using angle_t = Angle;

      vector_t translation{};
      angle_t alpha{};
      angle_t beta{};
      angle_t gamma{};
   };

   template <typename Vector, typename Angle>
   auto rotate_z_axis(transformation<Vector, Angle>& transform, Angle angle) noexcept 
   {
      transform.gamma = angle;
   };

   struct coordinate_system
   {
      //! \todo 
      //! \todo origin
   };
}

TEST_CASE("geometry", "[core]")
{
   using namespace sec21;
   using namespace boost::geometry;
   using namespace boost::math::constants;

   using point_t = boost::geometry::model::point<int, 3, boost::geometry::cs::cartesian>;

   SECTION("test point_t - quality assurance")
   {
      point_t pt1{ 1, 1, 1 };
      point_t pt2{ 2, 2, 2 };

      REQUIRE(distance(pt1, pt2) == Approx(1.7320508076));
   }
   SECTION("test glm::vec3 adapter")
   {
      glm::vec3 pt1{ 1, 1, 1 };
      glm::vec3 pt2{ 2, 2, 2 };

      REQUIRE(distance(pt1, pt2) == Approx(1.7320508076));


   }
   SECTION("downcast boost::geometry::point to glm::vec2")
   {
      model::point<double, 2, cs::cartesian> pt1{ 0.7, 42.42 };
      auto result = downcast<glm::vec2>(pt1);

      static_assert(std::is_same_v<decltype(result), glm::vec2>);
      REQUIRE(result.x == Approx(0.7));
      REQUIRE(result.y == Approx(42.42));
   }
   SECTION("downcast boost::geometry::point to glm::vec3")
   {
      point_t pt1{ 1, 2, 3 };
      auto result = downcast<glm::vec3>(pt1);

      static_assert(std::is_same_v<decltype(result), glm::vec3>);
      REQUIRE(result.x == Approx(1.0));
      REQUIRE(result.y == Approx(2.0));
      REQUIRE(result.z == Approx(3.0));
   }

   glm::vec3 input{ 0.125, 0.77, 1.0 };

   using transformation_t = transformation<glm::vec3, float>;

   const auto to_coordinate_system = [](glm::vec3 const& vec, auto const& sys) -> glm::vec3 
   {
      // R = R_z * R_y * R_x
      // p' = R * T * p
      auto t = glm::translate(glm::mat4{ 1.f }, sys.translation);
      auto r_x = glm::rotate(glm::mat4{ 1.f }, sys.alpha, { 1, 0, 0 });
      auto r_y = glm::rotate(glm::mat4{ 1.f }, sys.beta, { 0, 1, 0 });
      auto r_z = glm::rotate(glm::mat4{ 1.f }, sys.gamma, { 0, 0, 1 });
      auto m = r_z * r_y * r_x * t;
      glm::vec4 in{ vec, 1.0f };
      auto r = m * in;
      return { r.x, r.y, r.z };
   };

   transformation_t c1{ {0,0,0}, 0.f, 0.f, 0.f };

   rotate_z_axis(c1, pi<float>());

   auto p1 = to_coordinate_system(input, c1);
   REQUIRE(p1.x == Approx(0.125));
   REQUIRE(p1.y == Approx(0.77));
   REQUIRE(p1.z == Approx(1.0));
}
