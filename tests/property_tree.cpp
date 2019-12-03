#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <sec21/reflection.h>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
namespace pt = boost::property_tree;

#include <string>
#include <type_traits>

struct properties
{
   int i;
   std::string s;
};

namespace sec21::reflection
{
   template <>
   inline auto metainfo<properties>() noexcept
   {
      return std::tuple{ 
         register_member{ "i", &properties::i },
         register_member{ "s", &properties::s } 
      };
   }
}
