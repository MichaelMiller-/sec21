#pragma once

#include <sec21/reflection.h>

#include <type_traits>

namespace sec21
{
   //! \brief serializer that works with sec21::reflection classes
   template <typename Archive, typename T, std::enable_if_t<reflection::is_registered_v<std::decay_t<T>>, int> = 0>
   void serialize(Archive& ar, T&& t)
   {
      reflection::for_each_member<std::decay_t<T>>([&](auto const& member) 
      {
         using value_t = typename std::decay_t<decltype(member)>::value_t;

         if constexpr (reflection::is_registered_v<value_t>)
            serialize<Archive, value_t>(ar, member.get(t));

         ar.write(member.name(), member.get(t));
      });
   }

   //! \brief deserializer that works with sec21::reflection classes
   template <typename Archive, typename T, std::enable_if_t<reflection::is_registered_v<std::decay_t<T>>, int> = 0>
   void deserialize(Archive& ar, T&& t)
   {
      reflection::for_each_member<std::decay_t<T>>([&](auto const& member) 
      {
#if 0
         std::decay_t<decltype(member)>::value_t tmp{};
         ar.read(member.name(), tmp);
         member.set(t, tmp);
#endif
      });
   }    
}

//! Alternatively for non-reflectet-classes spezialize serialize() and deserialize() for the concrete class 
//!
//! eg: Non Intrusive Version
//!
//! namespace sec21 {
//! template <typename Archive>
//! void serialize(Archive& ar, T const& obj) {}
//! } // end namespace 

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
