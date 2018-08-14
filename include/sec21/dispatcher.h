#pragma  once

#include <map>
#include <typeindex>
#include <type_traits>
#include <cassert>

namespace sec21 
{
   template <typename Key, typename ReturnType>
   struct dispatcher
   {
      using key_type = std::decay_t<Key>;
      using return_type = ReturnType;

      //! \todo func also can be a lambda 
      template <typename Func>//, typename = std::enable_if_t<std::is_function<T>::value> >
      void insert(key_type key, Func func) {
         storage.insert(
            std::make_pair(
               key,
               std::make_pair(reinterpret_cast<function_type>(func), std::type_index(typeid(func)))));
      }

      auto operator[] (Key key) const {
         //! \todo what if key is not found?
         return proxy<return_type>(storage.find(key)->second);
      }

   private:
      using function_type = ReturnType(*)();
      using value_type = std::pair<function_type, std::type_index>;

      std::map<Key, value_type> storage;

   private:
      template <typename R>
      struct proxy
      {
         value_type v;

         explicit proxy(value_type t) : v{ t } { }

         template <typename... Args>
         auto operator()(Args &&... args)
         {
            auto func_t = reinterpret_cast<R(*)(Args...)>(v.first);

            assert(v.second == std::type_index(typeid(func_t)));
            return func_t(std::forward<Args>(args)...);
         }
      };
   };
}