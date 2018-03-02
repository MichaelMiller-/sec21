#pragma once

#include <map>

#include "sec21/function_traits.h"
#include "sec21/policies/clear.h"

namespace sec21 
{
   template <typename Key, typename Value, typename ClearPolicy>
   class cache
   {
   public:
      using key_type = std::decay_t<Key>;
      using value_type = Value;

   private:
      using function_type = Value(*)();
      function_type fptr;

      std::map<key_type, value_type> storage;

   public:
      template <typename Func>
      constexpr explicit cache(Func func) : fptr{ reinterpret_cast<function_type>(func) }
      { }

      auto operator()(key_type key)
      {
         const auto it{ storage.find(key) };

         if (it != std::end(storage)) {
            return it->second;
         }

         auto func_t = reinterpret_cast<Value(*)(Key)>(fptr);
         // call ftor
         value_type result = func_t(key);
         storage.insert(std::make_pair(key, result));
         // call policy
         ClearPolicy::apply(storage);

         return result;
      }
   };

   namespace detail {
      template <typename R, typename... Args>
      constexpr auto make_function(R(*p)(Args...)) -> std::function<R(Args...)> {
         return { p };
      }
   }

   template <typename Policy, typename Func>
   constexpr auto make_cached_function(Func func)
   {
      auto func_t = detail::make_function(func);
      using return_type = typename function_traits<decltype(func_t)>::result_type;
      using arg0 = typename function_traits<decltype(func_t)>::arg<0>;
      using arg0_t = typename arg0::type;

      return cache<arg0_t, return_type, Policy>(func);
   }
}
