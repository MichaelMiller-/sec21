#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include "catch.hpp"

#include <sec21/reflection.h>
#include <sec21/serializer.h>
#include <sec21/archive/json.h>

#include <filesystem>
#include <string>

using namespace sec21;
using namespace std::literals::string_literals;

namespace fs = std::filesystem;

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
   const auto temporary_test_file{ "tmp.json"s };

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

TEST_CASE("nested reflection and json-serializer test", "[reflection]")
{
   const auto temporary_test_file{ "tmp2.json"s };

   SECTION("write some data into a temporary json file")
   {
      person_t p{ "hans dampf", 42, "test address 13456", { 1, "s" } };
      std::ofstream ofs{ temporary_test_file };

      REQUIRE_NOTHROW([&] {
         archive::json ar{ ofs };
         ar << p;
      }());
   }
}

//TEST_CASE("simple copy from one container to another") //, "[benchmark]")
//{
//   BENCHMARK("for-each with push_back()")
//   {
//      const auto input = std::vector<int>(100, 0);
//      auto result = std::vector<int>();
//      for (auto e : input)
//         result.push_back(e);
//      return result;
//   };
//   BENCHMARK("std::copy()")
//   {
//      const auto input = std::vector<int>(100, 0);
//      auto result = std::vector<int>();
//      std::copy(std::begin(input), std::end(input), std::back_inserter(result));
//      return result;
//   };
//}

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
