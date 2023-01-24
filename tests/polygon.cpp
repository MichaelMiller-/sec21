#include <catch.hpp>

#include <sec21/polygon.h>

template <typename T>
struct cross_section_vertex
{
   using value_t = T;
   T y{};
   T z{};

#ifdef __cpp_lib_three_way_comparison
   constexpr bool operator==(cross_section_vertex const&) const = default;
#else
   constexpr bool operator==(cross_section_vertex const& obj) const { return std::tie(y, z) == std::tie(obj.y, obj.z); }
#endif

   friend auto operator+(cross_section_vertex const& lhs, cross_section_vertex const& rhs) noexcept
   {
      return cross_section_vertex{lhs.y + rhs.y, lhs.z + rhs.z};
   }
};

#include <boost/qvm/vec_traits_defaults.hpp>

namespace boost::qvm
{
   template <typename T>
   struct vec_traits<cross_section_vertex<T>> : vec_traits_defaults<cross_section_vertex<T>, T, 2>
   {
      using scalar_type = T;

      template <auto I>
      static inline scalar_type& write_element(cross_section_vertex<T>& v)
      {
         if constexpr (I == 0)
            return v.y;
         if constexpr (I == 1)
            return v.z;
      }
   };
} // namespace boost::qvm

#include <nlohmann/json.hpp>

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

using vertex_t = cross_section_vertex<int>;

// rect
const auto test_polygon = R"({
"vertices": [
   {
      "y": -20,
      "z": -30
   },
   {
      "y": 0,
      "z": -30
   },
   {
      "y": 0,
      "z": 70
   },
   {
      "y": -20,
      "z": 70
   }
]
})"_json;

TEST_CASE("polygon class", "[sec21]")
{
   using namespace sec21;

   const auto input = test_polygon.get<polygon<vertex_t>>();

   REQUIRE(size(input.vertices) == 4);

   SECTION("extent") { REQUIRE(input.extent() == 240); }
   SECTION("area") { REQUIRE(input.area() == 2000); }
   SECTION("center")
   {
      const auto result = input.center();
      REQUIRE(result.y == -10);
      REQUIRE(result.z == 20);
   }
   SECTION("move polygon")
   {
      auto cp = input;
      cp.translate(vertex_t{10, -20});
      const auto result = cp.center();
      REQUIRE(result.y == 0);
      REQUIRE(result.z == 0);
   }
   SECTION("convert to triangle list")
   {
      const auto result = to_triangle_list(input);
      const auto expected = std::array<vertex_t, 6>{
         vertex_t{-20, -30}, {0, -30}, {0, 70}, {-20, -30}, {0, 70}, {-20, 70},
      };
      REQUIRE(size(result) == size(expected));
      REQUIRE(std::equal(begin(result), end(result), std::begin(expected)));
   }
}
