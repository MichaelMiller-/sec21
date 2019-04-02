#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <sec21/reflection.h>
#include <sec21/serializer.h>
#include <sec21/archive/json.h>

struct properties
{
   int z;
   std::string b;
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
         register_member{ "z", &properties::z },
         register_member{ "b", &properties::b } 
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


TEST_CASE("reflection and json-serializer test", "[reflection]")
{
   using namespace sec21;
   using nlohmann::json;

   //! \todo parse from string
   //const auto j2 = R"(
   //{
   //   "b": "any long string",
   //   "z": 43
   //})"_json;
    
   SECTION("write and read some testdata to a json file") 
   {
      properties p1{ 18, "test data" };
      std::ofstream ofs{"tmp.json" };

      REQUIRE_NOTHROW([&]{
         archive::json o{ ofs };
         o << p1;
      }());

      // read the written data
      properties read_data;
      std::ifstream ifs{ "tmp.json" };
      archive::json o{ ifs };
      o >> read_data;      
      
      REQUIRE(read_data.z == 18);
      REQUIRE(read_data.b == "test data");
   }

#ifndef WIN32
   //! \todo find file under windows
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
}
