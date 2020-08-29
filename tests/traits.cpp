#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <tuple>
#include <string>
#include <vector>
#include <type_traits>

template <typename T>
struct is_tuple : std::false_type {};

template <typename... Args>
struct is_tuple<std::tuple<Args...>> : std::true_type {};

template <typename T>
constexpr bool is_tuple_v = is_tuple<T>::value;

static_assert(is_tuple_v<std::tuple<int, float, int>>, "should be a true");
static_assert(is_tuple_v<std::tuple<>>, "should be a true");
static_assert(!is_tuple_v<int>, "should be a false");
static_assert(!is_tuple_v<std::string>, "should be a false");
static_assert(!is_tuple_v<double>, "should be a false");



template <template <typename...> typename Base, typename U>
struct is_instance_of : std::false_type {};

template <template <typename...> typename Base, typename... Args>
struct is_instance_of<Base, Base<Args...>> : std::true_type {};

template <template <typename...> typename Base, typename U>
constexpr bool is_instance_of_v = is_instance_of<Base, U>::value;

static_assert(is_instance_of_v<std::basic_string, std::string>, "must be a instance_of");
static_assert(is_instance_of_v<std::tuple, std::tuple<int, double>>, "must be a instance_of");
static_assert(!is_instance_of_v<std::tuple, std::vector<int>>, "should be false");
static_assert(!is_instance_of_v<std::vector, std::tuple<int, double>>, "should be false");


TEST_CASE("traits", "[sec21][traits]") 
{
   SUCCEED("Nothing to test. Compiletime-Tests");
}
