#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <string>
using namespace std::literals;

TEST_CASE("access a std::array", "[sec21][orm_compiler]")
{
   const auto input = R"(
namespace sec21::structural_analysis
{
    [[Entity]]
    struct structural_point
    {
        [[Column]]
        std::string name  ;
        
        [[Column]][[SqlString]]
        boost::uuids::uuid tag;
        
        [[Column]]
        double coordinate_x;
        double coordinate_y;
        double coordinate_z;
    };
})"s;

}
