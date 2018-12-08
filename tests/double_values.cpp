#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <vector>
#include <iterator>
#include <utility>

namespace sec21
{
   template <size_t N, typename T, typename Iterator, typename TransformFunc>
   [[nodiscard]] auto double_values(Iterator first, Iterator last, TransformFunc func)
   {
      const auto s{ std::distance(first, last) };
      std::vector<T> result(s * N);

      for (auto i = 0; i < s; ++i)
      {
         //! \todo loop unrolling
         for (size_t j{ 0 }; j < N; ++j)
            result[i * N + j] = func(*first);
         ++first;
      }
      return result;
   }

   //! \brief doubles the value in the input sequence N-times eg: N=2; {1,2,3} -> {1,1,2,2,3,3}
   template <size_t N, typename Iterator>
   [[nodiscard]] auto double_values(Iterator first, Iterator last)
   {
      using T = typename std::iterator_traits<Iterator>::value_type;
      return double_values<N, T>(first, last, [](auto && e){ return e; });
   }

   template <size_t N, typename Container>
   [[nodiscard]] auto double_values(Container const& c) 
   { 
      return double_values(std::begin(c), std::end(c)); 
   }
}

#include <tuple>

struct foo
{
   std::string s{};
   int i{};
   double d{};

   friend bool operator == (foo const& lhs, foo const& rhs) noexcept {
      return lhs.i == rhs.i && lhs.d == rhs.d && lhs.s == rhs.s;
   }
};

#include <utility>

template <size_t> struct alternate_dir;

template <> struct alternate_dir<2>
{
    using direction_t = std::array<int, 2>;
    static constexpr direction_t value = { 0, 1};
};
template <> struct alternate_dir<3>
{
    using direction_t = std::array<int, 3>; 
    static constexpr direction_t value = { 0, 1, 2};
};

TEST_CASE("double_values", "[core]")
{
   using namespace sec21;

   SECTION("simple test")
   {
      std::vector<int> input{ 1, 2, 3 };
      auto result = double_values<4>(input.begin(), input.end());
      const decltype(result) expected{ 1,1,1,1,2,2,2,2,3,3,3,3 };
      REQUIRE(result == expected);
   }
   SECTION("transformation")
   {
      std::vector<int> input{ 1, 2, 3 };
      auto result = double_values<2, std::string>(input.begin(), input.end(), [](auto && e){ return std::to_string(e); });
      const decltype(result) expected{ "1","1","2","2","3","3" };
      REQUIRE(result == expected);
   }
   SECTION("transformation into struct")
   {
      std::vector<int> input{ 1, 2, 3 };

      using T = std::tuple<std::string, int, double>;


      // auto result = double_values<2, T>(input.begin(), input.end(), [&direction](auto && e){ 
        
      //    auto result = std::tuple{ std::to_string(e), e * (*std::begin(direction)), 3.14}; 
      //    std::rotate(std::begin(direction), std::begin(direction) + 1, std::end(direction));
      //    return result;
      // });

      auto result = double_values<2, foo>(input.begin(), input.end(), [](auto && e){ return foo{ std::to_string(e), e, 3.14}; });
      const decltype(result) expected{ 
         { "1", 1, 3.14 },
         { "1", 1, 3.14 },
         { "2", 2, 3.14 },
         { "2", 2, 3.14 },
         { "3", 3, 3.14 },
         { "3", 3, 3.14 } };
      REQUIRE(result == expected);
   }   
   SECTION("transformation into tuple")
   {
      std::vector<int> input{ 1, 2, 3 };

      using T = std::tuple<std::string, int, double>;

      auto direction = alternate_dir<2>::value;

      auto result = double_values<2, T>(input.begin(), input.end(), [&direction](auto && e){ 
        
         auto result = std::tuple{ std::to_string(e), e * (*std::begin(direction)), 3.14}; 
         std::rotate(std::begin(direction), std::begin(direction) + 1, std::end(direction));
         return result;
      });

      const decltype(result) expected{ 
         { "1", 0, 3.14 },
         { "1", 1, 3.14 },
         { "2", 0, 3.14 },
         { "2", 2, 3.14 },
         { "3", 0, 3.14 },
         { "3", 3, 3.14 } };
      REQUIRE(result == expected);
   }
}

