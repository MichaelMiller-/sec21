#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/range/algorithm/copy.hpp>

#include <sec21/flat_matrix.h>

TEST_CASE("flatten matrix to a std::vector", "[core]")
{
   using namespace sec21;
   namespace bnu = boost::numeric::ublas;
   
   SECTION("empty matrix")
   {
      bnu::matrix<int> input{};
      auto result = flat_matrix(input);

      REQUIRE(std::size(result) == 0);
   }
   SECTION("example 3x3 matrix")
   {
      bnu::matrix<int> input(3, 3, 1);
      auto result = flat_matrix(input);

      const std::vector<int> expected{ 1, 1, 1, 1, 1, 1, 1, 1, 1 };
      REQUIRE(std::size(result) == std::size(expected));
      REQUIRE(result == expected);
   }
#if 0   
   SECTION("example 1x5 matrix")
   {
      std::vector<std::vector<int>> helper_input{ 
         std::vector{ 1, 2, 3, 4, 5 },
      };
      auto input = helper::make_matrix(1, 5, helper_input);
      auto result = flat_matrix(input);

      const std::vector<int> expected{ 1, 2, 3, 4, 5 };
      REQUIRE(std::size(result) == std::size(expected));
      REQUIRE(result == expected);
   }
   SECTION("example 5x1 matrix")
   {
      std::vector<std::vector<int>> helper_input{ 
         std::vector{ 1 },
         std::vector{ 2 },
         std::vector{ 3 },
         std::vector{ 4 },
         std::vector{ 5 }
      };
      auto input = helper::make_matrix(5, 1, helper_input);
      auto result = flat_matrix(input);

      const std::vector<int> expected{ 1, 2, 3, 4, 5 };
      REQUIRE(std::size(result) == std::size(expected));
      REQUIRE(result == expected);
   }
   SECTION("example 2x4 matrix")
   {
      std::vector<std::vector<int>> helper_input{ 
         std::vector{ 1, 2, 3, 4 },
         std::vector{ 5, 6, 7, 8 }
      };
      auto input = helper::make_matrix(2, 4, helper_input);
      auto result = flat_matrix(input);

      const std::vector<int> expected{ 1, 2, 3, 4, 5, 6, 7, 8 };
      REQUIRE(std::size(result) == std::size(expected));
      REQUIRE(result == expected);
   }
#endif   
}
