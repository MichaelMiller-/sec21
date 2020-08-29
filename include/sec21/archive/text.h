#pragma once

#include <type_traits>

#include <nlohmann/json.hpp>

#include <sec21/reflection.h>

namespace sec21
{
   namespace archive
   {
      // template policies like: newline at the end, formats
      template <typename Stream>
      struct text
      {
         Stream stream;

         template <typename U>
         explicit text(U && u) noexcept : stream{ u }
         { }

         template <typename U>
         void write(std::string_view name, U&& u) 
         {
               stream << name << ":" << u;
         }
         template <typename U>
         void read(std::string_view name, U& u) 
         {
               // stream >> name >> ":" >> u;
         }            
      };
   }
}

namespace sec21
{
   //! \brief serializer that works with sec21::reflection classes
   template <typename Archive, typename T> //, std::enable_if_t<reflection::is_registered_v<std::decay_t<T>>, int> = 0>
#ifdef __cpp_conditional_explicit
   explicit(reflection::is_registered_v<std::decay_t<T>>)
#endif
   void serialize(Archive& ar, T&& t)
   {
      reflection::for_each_member<std::decay_t<T>>([&](auto const& member) 
      {
         ar.write(member.name(), member.get(t));
      });
   }

   //! \brief deserializer that works with sec21::reflection classes
   template <typename Archive, typename T, std::enable_if_t<reflection::is_registered_v<std::decay_t<T>>, int> = 0>
#ifdef __cpp_conditional_explicit
   explicit(reflection::is_registered_v<std::decay_t<T>>)
#endif
   void deserialize(Archive& ar, T&& t)
   {
      //static_assert(std::is_default_constructible_v<T>, "Type T has to be default-constructible");
      reflection::for_each_member<std::decay_t<T>>([&](auto const& member) 
      {
         ar.read(member.name(), member.get(t));
      });
      //return result;

   }    
}

// spezialisierung ist notwendig wenn keine reflection existiert 
// wichtig: bekommt immer vorang zu einer klasse die eine reflection besitzt
//
// eg: Non Intrusive Version
// namespace sec21 {
// template <typename Archive>
// void serialize(Archive& ar, T const& obj) {}
// } // end namespace 

template <typename Archive, typename T, std::enable_if_t<sec21::reflection::is_registered_v<std::decay_t<T>>, int> = 0>
auto operator << (Archive& ar, T const& t)
{
   sec21::serialize(ar, t);
   ar.flush();
}    
template <typename Archive, typename T, std::enable_if_t<sec21::reflection::is_registered_v<std::decay_t<T>>, int> = 0>
auto operator >> (Archive& ar, T& t)
{
   ar.open();
   sec21::deserialize(ar, t);
}
