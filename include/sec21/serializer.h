#pragma once

#include <type_traits>

#include <sec21/reflection.h>
#include <nlohmann/json.hpp>

namespace sec21
{
   template <typename T>
   T deserialize(nlohmann::json const& j)
   {
      static_assert(std::is_default_constructible_v<T>, "Type has to be default-constructible");

      T result;
      reflection::for_each_member<T>([&](auto const& member) 
      { 
         j.at(member.name().data()).get_to(member.get(result)); 
      });
      return result;
   }

   template <typename T>
   nlohmann::json serialize(T const& obj)
   {
      nlohmann::json j;
      reflection::for_each_member<T>([&](auto const& member)
      {
         j[member.name().data()] = member.get(obj);
      });
      return j;
   }
}
