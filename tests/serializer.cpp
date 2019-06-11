#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <sec21/reflection.h>
#include <sec21/serializer.h>
#include <sec21/archive/json.h>

#if __GNUC__ == 7
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#else
#include <filesystem>
namespace fs = std::filesystem;
#endif

#include <string>

struct properties
{
   int i;
   std::string s;
};

template <typename T>
struct person
{
   std::string name;
   int age;
   std::string address;
   //! \todo reflect subtrees
   T data;
};

using person_t = person<properties>;

namespace sec21::reflection
{
   template <>
   inline auto metainfo<properties>()
   {
      return std::tuple{ 
         register_member{ "i", &properties::i },
         register_member{ "s", &properties::s } 
      };
   }

   template <>
   inline auto metainfo<person_t>()
   {
      return std::tuple{ 
         register_member{ "name", &person_t::name }, 
         register_member{ "age", &person_t::age },
         register_member{ "address", &person_t::address },
         register_member{ "data", &person_t::data }
      };
   }
}

TEST_CASE("simple reflection and json-serializer test", "[reflection]")
{
   constexpr std::string_view temporary_test_file{ "tmp.json" };

   using namespace sec21;

   SECTION("write some data into a temporary json file") 
   {
      properties p{ 42, "test data" };
      std::ofstream ofs{ temporary_test_file };

      REQUIRE_NOTHROW([&]{
         archive::json ar{ ofs };
         ar << p;
      }());
   }
   SECTION("read testdata from temporary json file") 
   {
      std::ifstream ifs{ temporary_test_file };
      archive::json ar{ ifs };

      properties p;
      ar >> p;

      REQUIRE(p.i == 42);
      REQUIRE(p.s == "test data");
   }
}

#if 0
TEST_CASE("nested reflection and json-serializer test", "[reflection]")
{
   constexpr std::string_view temporary_test_file{ "tmp1.json" };

   using namespace sec21;

   SECTION("write some data into a temporary json file") 
   {
      person_t p{ "Ned Flanders", 42, "Mainstreet 123", { 987, "additional test string" } };
      std::ofstream ofs{ temporary_test_file };

      //! \todo output is wrong
      REQUIRE_NOTHROW([&]{
         archive::json ar{ ofs };
         ar << p;
      }());
   }
   //SECTION("read testdata from temporary json file") 
   //{
   //   std::ifstream ifs{ temporary_test_file };
   //   archive::json ar{ ifs };

   //   person_t in;
   //   ar >> in;

   //   REQUIRE(in.i == 42);
   //   REQUIRE(in.s == "test data");
   //}
}
#endif

//! \todo 2019-04-03: there is a problem with finding the datafiles -> buildsystem
#if 0
   SECTION("read testdata from a json file") 
   {
      // read the written data
      properties read_data;
      std::ifstream ifs{ "test_properties1.json" };
      archive::json ari{ ifs };
      ari >> read_data;      
      
      REQUIRE(read_data.z == 42);
      REQUIRE(read_data.b == "json serializer unit test data");
   }

   //! \todo parse from string
   //const auto j2 = R"(
   //{
   //   "b": "any long string",
   //   "z": 43
   //})"_json;

   SECTION("read some testdata from filesystem file") 
   {
      properties p2;
      std::ifstream ifs{ "test_properties2.json" };

      REQUIRE_NOTHROW([&]{
         archive::json o{ ifs };
         o >> p2;
      }());
      REQUIRE(p2.z == 42);
      REQUIRE(p2.b == "json serializer unit test data");
   }
#endif
