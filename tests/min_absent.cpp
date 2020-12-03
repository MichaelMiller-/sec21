#include <catch.hpp>

#include <algorithm>

namespace sec21
{
    template <std::forward_iterator Iterator, typename T = std::iter_value_t<Iterator>>
    constexpr auto min_absent(Iterator first, Iterator last, T initial = {}) noexcept
    {
        using difference_t = decltype(initial - initial);
        while (last != first)
        {
            auto const half = (std::distance(first, last) + 1) / 2;
            T const m = initial + static_cast<difference_t>(half);
            auto const pos = std::partition(first, last, [&](auto const &x) { return x < m; });
            if (pos == std::next(first, half))
            {
                first = pos;
                initial = m;
            }
            else
                last = pos;
        }
        return initial;
    }
} // namespace sec21

TEST_CASE("find the next absent element missing in a sorted sequence", "[sec21][core]")
{
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