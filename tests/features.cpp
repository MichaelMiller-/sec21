#include <catch.hpp>

#include <type_traits>
#include <vector>
#include <algorithm>
#include <functional>
#include <tuple>
#include <set>
#include <numeric>
#include <cmath>
#include <cstdint>

using namespace std::literals;

///
/// Test C++ Features; Basically a HowTo
///
/// Boilerplate
///
int do_something(const int i) 
{
   return 34 + i;
}

struct S
{
   std::string name{};
   int i{ 2 };
   int j{ 4 };
   int k{ 8 };

   int do_something(const int v) const { return j + v; }
};

auto make_S() -> S
{
   S result{};
   result.name = "bar";
   result.i = 5;
   result.j = 6;
   result.k = 7;
   return result;
}

class Employee
{
   int id{ 0 };
   std::string name{};

public:
   explicit Employee(int id_, std::string const& name_) : id{ id_ }, name{ name_ } {}

   int getId() const { return id; }
   std::string getName() const { return name; }
};

struct CompareId
{
   using is_transparent = void;

   bool operator()(Employee const& employee1, Employee const& employee2) const
   {
      return employee1.getId() < employee2.getId();
   }
   bool operator()(int id, Employee const& employee) const
   {
      return id < employee.getId();
   }
   bool operator()(Employee const& employee, int id) const
   {
      return employee.getId() < id;
   }
};

//! \brief creates a std::set with a is_transparent comparator
template <typename T, typename... Comparator>
auto make_set(Comparator&&... comparator)
{
   struct Compare : std::decay_t<Comparator>...
   {
      using std::decay_t<Comparator>::operator()...;
      using is_transparent = void;
   };
   return std::set<T, Compare>{ Compare{ std::forward<Comparator>(comparator)... } };
}

/// 
///

// language feature: c++17
TEST_CASE("Aggregate Initialization", "[features]")
{
#ifdef __cpp_aggregate_bases
   S s1{ { "foo" }, 7, 8, 9 };

   REQUIRE(s1.name == "foo");
   REQUIRE(s1.i == 7);
   REQUIRE(s1.j == 8);
   REQUIRE(s1.k == 9);
#else
   WARN("Aggregate initialization is not supported");
#endif
}

#ifdef __cpp_fold_expressions
template<typename T, typename... Args>
void push_back_vec(std::vector<T>& v, Args&&... args)
{
   (v.push_back(args), ...);
}

template <typename... Args>
auto do_stuff(Args... args)
{
   return ([args]() { return args + 2; }() + ...);
}
#endif

// language feature: c++17
TEST_CASE("Fold expression", "[features]")
{
#ifdef __cpp_fold_expressions
   std::vector<int> result{};
   push_back_vec(result, 2, 3, 8, 9, 3);

   std::vector<int> expected{ 2, 3, 8, 9, 3 };
   REQUIRE(size(result) == size(expected));
   REQUIRE(std::equal(begin(result), end(result), begin(expected)));

   SECTION("lambdas in fold expressions")
   {
      REQUIRE(do_stuff(4, 4, 30, 17, 13) == 78);
   }
#else
   WARN("Aggregate initialization is not supported");
#endif
}

// language feature: c++17
TEST_CASE("Structured bindings", "[features]")
{
#ifdef __cpp_structured_bindings
   auto [n, i, j, k] = make_S();
   REQUIRE(n == "bar"s);
   REQUIRE(i == 5);
   REQUIRE(j == 6);
   REQUIRE(k == 7);
#else
   WARN("Structured bindings is not supported");
#endif
}

// language feature: c++20
TEST_CASE("Designated initializer", "[features]")
{
#ifdef __cpp_designated_initializers
   S s1{ 
      .j = 1, 
      .i = 42, 
      .k = [](auto v) { return v * v; }(4)
   };

   REQUIRE(s1.i == 42);
   REQUIRE(s1.j == 1);
   REQUIRE(s1.k == 16);
#else
   WARN("Designated initializer is not supported");
#endif
}

// language feature: c++20
TEST_CASE("lambdas in a unevaluated context and default constructable stateless lambdas", "[features]")
{
#if __cplusplus > 201803L
   auto file = std::unique_ptr<FILE, decltype([](FILE* p) { fclose(p) })>(fopen("somefile.txt", "w"));
   SUCCEED("Nothing to test. Compiletests");
#else
   WARN("lambdas in a unevaluated context are not supported");
#endif
}

///
///

// library feature: c++14
TEST_CASE("Heterogeneous lookup", "[features]")
{
#ifdef __cpp_lib_generic_associative_lookup
   SECTION("use dedicated compare class")
   {
      std::set<Employee, CompareId> employees{
         Employee(1, "John"),
         Employee(2, "Bill")
      };
      REQUIRE(employees.find(2)->getName() == "Bill");
   }
   SECTION("use generic version of comparator")
   {
      auto employees = make_set<Employee>(
         [](Employee const& lhs, Employee const& rhs) { return lhs.getId() < rhs.getId(); },
         [](Employee const& lhs, auto const& rhs) { return lhs.getId() < rhs; },
         [](auto const& lhs, Employee const& rhs) { return lhs < rhs.getId(); }
      );

      employees.insert(Employee(1, "John"));
      employees.insert(Employee(2, "Bill"));
      employees.insert(Employee(3, "Peter"));

      REQUIRE(employees.find(1)->getName() == "John");
      REQUIRE(employees.find(2)->getName() == "Bill");
      REQUIRE(employees.find(3)->getName() == "Peter");
      REQUIRE(employees.find(Employee(3, "Peter"))->getId() == 3);
   }
#else
   WARN("Heterogeneous comparison lookup is not supported");
#endif
}

// library feature: c++17
TEST_CASE("std::invoke", "[features]")
{
#ifdef __cpp_lib_invoke
   REQUIRE(std::invoke(&do_something, 5) == 39);
   S s{};
   REQUIRE(std::invoke(&S::do_something, s, 3) == 7);
   REQUIRE(std::invoke(&S::j, s) == 4);
#else
   WARN("std::invoke is not supported");
#endif
}

// library feature: c++17
TEST_CASE("std::exchange", "[features]")
{
#ifdef __cpp_lib_exchange_function
   /// exchange vs swap
   int x = 2;
   // A <- B <- C
   auto y = std::exchange(x, 4); // --> y == 2; x == 4
   // A -> B; A <- B; std::swap()
   REQUIRE(x == 4);
   REQUIRE(y == 2);
#else
   WARN("std::exchange is not supported");
#endif
}

// library feature: c++17
TEST_CASE("std::clamp", "[cpp]")
{
#ifdef __cpp_lib_clamp
   int i = 3;
   REQUIRE(std::clamp(i, 2, 5) == 3);
   REQUIRE(std::clamp(i, 4, 5) == 4);
#else
   WARN("std::clamp is not supported");
#endif
}

// library feature: c++17
TEST_CASE("boyer moore searcher", "[features]")
{
#ifdef __cpp_lib_boyer_moore_searcher
   const std::vector<int> input = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
   const std::vector<int> lookup = { 3, 4, 5 };
   const auto it = std::search(
      std::begin(input), 
      std::end(input), 
      std::boyer_moore_searcher{ std::begin(lookup), std::end(lookup) });

   REQUIRE(std::distance(std::begin(input), it) == 2);
#else
   WARN("std::boyer_moore_searcher is not supported");
#endif
}

// library feature: c++20
TEST_CASE("midpoint", "[features]")
{
#ifdef __cpp_lib_interpolate
   REQUIRE(std::midpoint(2, 8) == 6);
#else
   WARN("std::midpoint is not supported");
#endif
}

// library feature: c++20
TEST_CASE("linear interpolation", "[features]")
{
#ifdef __cpp_lib_interpolate
   REQUIRE(std::lerp(1.0, 2.0, 0.5) == Approx(1.5));
#else
   WARN("std::lerp is not supported");
#endif
}

// library feature: c++20
TEST_CASE("starts_with and end_with", "[features]")
{
#ifdef __cpp_lib_starts_ends_with
   std::string_view input = "Hello World";

   REQUIRE(input.starts_with("H"));
   REQUIRE(input.starts_with("Hello"));
   REQUIRE(input.end_with("d"));
   REQUIRE(input.end_with("World"));
#else
   WARN("starts_with() and ends_with() is not supported");
#endif
}

#ifdef __cpp_lib_math_constants
   #include <numbers>
#endif
// library feature: c++20
TEST_CASE("math constants", "[features]")
{
#ifdef __cpp_lib_math_constants
   STATIC_REQUIRE(std::numbers::pi == Approx(3.14));
#else
   WARN("math constants are not available");
#endif
}

// library feature: c++20
TEST_CASE("shift elements in a range", "[features]")
{
#ifdef __cpp_lib_shift
   std::vector<int> result{ 1, 2, 3, 4, 5 };
   auto it = std::shift_left(std::begin(result), std::end(result), 2);

   std::vector<int> expected1{ 3, 4, 5, 4, 5 };
   REQUIRE(std::size(result) == std::size(expected1));
   REQUIRE(std::equal(std::begin(result), std::end(result), std::begin(expected1)));

   result.erase(it, std::end(result))
   std::vector<int> expected2{ 3, 4, 5 };
   REQUIRE(std::size(result) == std::size(expected2));
   REQUIRE(std::equal(std::begin(result), std::end(result), std::begin(expected2)));
#else
   WARN("math constants are not available");
#endif
}

// library feature: c++20
TEST_CASE("Uniform container erasure", "[features]")
{
#ifdef __cpp_lib_erase_if
   std::vector<int> result{ 1, 5, 3, 4, 6, 5, 7, 9, 6, 7, 5, 3 };
   // basically replaces the "remove-erase-idiom"
   std::erase(result, 5);

   const std::vector<int> expected = { 1, 3, 4, 6, 7, 9, 6, 7, 3 };
   REQUIRE(std::equal(std::begin(result), std::end(result), std::begin(expected)));
#else
   WARN("Uniform container erasure is not supported");
#endif
}
