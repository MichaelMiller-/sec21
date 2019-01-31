#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <vector>
#include <algorithm>

template <typename Function, typename Projection>
class projection_func
{
   Function    m_function;
   Projection  m_projection;

public:
   projection_func(Function function, Projection projection) noexcept
      : m_function{ std::move(function) }
      , m_projection{ std::move(projection) }
   {}

   projection_func() = delete;

   template <typename... Args>
   decltype(auto) operator() (Args &&... args) const 
   {
      return std::invoke(m_function, std::invoke(m_projection, std::forward<Args>(args))...);
   }
};

TEST_CASE("projection wrapper function", "[core]")
{
   struct foo
   {
      int a;
      double b;
   };

   SECTION("simple test")
   {
      std::vector<foo> result{
        { 1, 3.12 },
        { 4, 3.12 },
        { 5, 76.7 },
        { 3, 7.2 },
        { 2, 0.82 }
      };

      std::sort(std::begin(result), std::end(result), projection_func{ std::less{}, &foo::a });

      const std::vector<foo> expected{
        { 1, 3.12 },
        { 2, 0.82 },
        { 3, 7.2 },
        { 4, 3.12 },
        { 5, 76.7 }
      };

      REQUIRE(std::equal(
         std::begin(result), 
         std::end(result), 
         std::begin(expected), 
         std::end(expected),
         // only member foo::a is needed for this test
         [](auto const& lhs, auto const& rhs){ return lhs.a == rhs.a; }) == true);
   }
}