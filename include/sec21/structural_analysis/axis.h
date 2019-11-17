#pragma once

#include <sec21/enumerate.h>

#include <vector>
#include <map>

namespace sec21::structural_analysis
{
   enum class Axis
   {
      X = 0,
      Y = 1,
      Z = 2
   };

   // template <typename T>
   // class proxy_axis
   // {
   //    T& value;
   // public:
   //    explicit proxy_axis(T& t) : value{t} {}

   //    auto operator[](Axis a) const
   //    {
   //       return value[static_cast<std::size_t>(a)];
   //    }
   //    auto& operator[](Axis a)
   //    {
   //       return value[static_cast<std::size_t>(a)];
   //    }   
   // };

   // key -> axis -> value
   template <typename Key, typename Value, int Dimension = 2>
   class key_axis_value_table
   {
   public:   
      using key_t = Key;
      using value_t = std::array<Value, Dimension>;
      
   private:   
      std::vector<value_t> values;
      std::map<key_t, std::size_t> keys;

      class proxy
      {
         value_t& value;
      public:
         explicit proxy(value_t& t) : value{t} {}

         auto operator[](Axis a) const 
         {
            return value[static_cast<std::size_t>(a)];
         }
         auto& operator[](Axis a)
         {
            return value[static_cast<std::size_t>(a)];
         }   
      };

   public:
      auto insert_or_assign(key_t key, value_t&& v)
      {
         if (auto it = keys.find(key); it != std::end(keys))
         {
            values[it->second] = v;
            return std::pair{ it, false };
         }
         values.push_back(v);
         return keys.insert({key, std::size(values)-1});
      }

      //! \brief aquire space
      //! \todo need a projection for the iterator value -> data -> projection{ data.id }
      template <typename Iterator>
      auto fill(Iterator first, Iterator last)
      {
         const auto n = std::distance(first, last);
         values.resize(n);
         for_each_indexed(first, last, [this](auto i, auto&& e){
            keys[e] = i;
         });
      }

      auto operator[](key_t key) -> proxy
      {
         if (auto it = keys.find(key); it != std::end(keys))      
            return proxy{values[it->second]};

         throw std::invalid_argument("invalid key");
      }

      // auto flat_values() -> std::vector<Value>
      // {
      //    return {};
      // }
   };

}

