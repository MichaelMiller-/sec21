#include <catch.hpp>

#include <sec21/min_absent.h>

TEST_CASE("find the next absent element missing in a sorted sequence", "[sec21][core]")
{
    using namespace sec21;

    SECTION("empty sequence")
    {
        std::vector<int> in{};
        REQUIRE(min_absent(std::begin(in), std::end(in)) == 0);
    }
    SECTION("even-length sequence with a gap")
    {
        std::vector<int> in{1, 2, 3, 5, 6, 7, 8, 9};
        REQUIRE(min_absent(std::begin(in), std::end(in), *std::begin(in)) == 4);
    }
    SECTION("odd-length sequence with a gap")
    {
        std::vector<int> in{0, 1, 2, 3, 4, 6, 7, 8, 9};
        REQUIRE(min_absent(std::begin(in), std::end(in), *std::begin(in)) == 5);
    }
    SECTION("sequence stating at 0 without a gap")
    {
        std::vector<int> in{0, 1, 2, 3, 4, 5, 6, 7};
        REQUIRE(min_absent(std::begin(in), std::end(in), *std::begin(in)) == 8);
    }
    SECTION("sequence stating at 2 without a gap")
    {
        std::vector<int> in{2, 3, 4, 5, 6, 7, 8, 9};
        REQUIRE(min_absent(std::begin(in), std::end(in), *std::begin(in)) == 10);
    }
    SECTION("sequence with a N>1 gap")
    {
        std::vector<int> in{0, 1, 5, 6, 7};
        REQUIRE(min_absent(std::begin(in), std::end(in), *std::begin(in)) == 2);
    }
}