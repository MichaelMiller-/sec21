#ifndef SEC21_STRUCTURAL_LOAD_CASE_H
#define SEC21_STRUCTURAL_LOAD_CASE_H

#include <boost/config.hpp>

#ifdef SEC21_USE_BOOST_SERIALIZATION
   #include <boost/archive/text_oarchive.hpp>
   #include <boost/archive/text_iarchive.hpp>
#endif

namespace sec21
{
   namespace structural_analysis
   {
      class loadcase
      {
      public:
         bool valid() const
         { 
            return true;
         }

#ifdef SEC21_USE_BOOST_SERIALIZATION
      private:
         friend class boost::serialization::access;
         // When the class Archive corresponds to an output archive, the
         // & operator is defined similar to <<.  Likewise, when the class Archive
         // is a type of input archive the & operator is defined similar to >>.
         template<
            class Archive>
         void serialize(Archive & ar, const unsigned int version)
         {
            //! \todo 
         }
#endif

      private:
      };
   }
}

#ifdef SEC21_USE_LUA

extern "C"
{
   #include "lua.h"
   #include "lauxlib.h"
   #include "lualib.h"
}

#include <LuaIntf/LuaIntf.h>

namespace LuaIntf
{
/*   
   template <> struct LuaTypeMapping<sec21::structural_analysis::support>
   {
      static sec21::structural_analysis::support get(lua_State* L, int index)
      {
         using namespace sec21::structural_analysis;

         const auto ref = LuaIntf::LuaRef(L, index);

         if (!ref.isValid())
            throw std::runtime_error("invalid argument for support");

         const auto i = ref.toValue<std::underlying_type<support_type>::type>();

         return support(support_type(i));
      }
   };
*/
} // namespace LuaIntf

namespace sec21
{
   static void register_lua_loadcase(lua_State* L)
   {
      using namespace sec21::structural_analysis;
      using namespace LuaIntf;

//      LuaBinding(L).beginModule("framework")
//      .endModule();
   } // register_lua_loadcase
} // namspace sec21

#endif // SEC21_USE_LUA

#endif // SEC21_STRUCTURAL_LOAD_CASE_H