#include "catch.hpp"

#include <array>

#if __has_include(<experimental/reflect>)
#include <experimental/reflect>

namespace reflect = std::experimental::reflect;

template <template<typename> typename Trait, reflect::ObjectSequence Sequence, auto... Is>
consteval auto make_object_sequence_array(std::index_sequence<Is...>)
{
   return std::array { Trait<reflect::get_element_t<Is, Sequence>>::value... };
}

template <typename T>
consteval auto enum_names()
   requires std::is_enum_v<T>
{
   using reflected_enum_t = reflexpr(T);
   using enum_enumerators_t = reflect::get_enumerators_t<reflected_enum_t>;
   constexpr auto n = reflect::get_size_v<enum_enumerators_t>;
   using sequence = std::make_index_sequence<n>;
   return make_object_sequence_array<reflect::get_name, enum_enumerators_t>(sequence{});
}
#define HAS_REFLECTION 1
#else
#define HAS_REFLECTION 0
#endif

enum class DaysOfTheWeek
{
   Monday,
   Tuesday,
   Wednesday,
   Thursday,
   Friday,
   Saturday,
   Sunday
};

TEST_CASE("convert enum to string at compiletime", "[sec21][core]")
{
#if HAS_REFLECTION
   constexpr auto names = enum_names<DaysOfTheWeek>();
   constexpr auto expected = std::array{"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};

   STATIC_REQUIRE(size(names) == size(expected));
   STATIC_REQUIRE(std::equal(begin(names), end(name), begin(expected)));
#else
   WARN("c++ reflection header is not found");
   REQUIRE(true);
#endif
}
