#pragma once

#include <tuple>
#include <string_view>
#include <stdexcept>
#include <type_traits>
#include <functional>

namespace sec21::reflection
{
   //! every class has to spezialize this function
   //!
   //! example:
   //! template <>
   //! inline auto metainfo<person>() 
   //! {
   //!    return std::tuple(register_member("name", &person::name), register_member("age", &person::age));
   //! }
   //!
   template <typename Class>
   inline constexpr auto metainfo() { return std::tuple{}; }


   template <typename Class, typename T>
   class register_member
   {
   public:
      using member_ptr_t = T Class::*;
      using value_t = T;
   
   private:
      const std::string_view  m_name{};
      member_ptr_t            m_ptr{nullptr};

   public:
      explicit register_member(std::string_view name, member_ptr_t ptr) noexcept
         : m_name{ name }
         , m_ptr{ ptr }
      { }

      auto name() const noexcept { return m_name; }
      
      auto& get(Class& obj) const
      {
         if (m_ptr)
            return obj.*m_ptr;

         throw std::runtime_error("invalid member pointer");
      }

      auto get(Class const& obj) const
      {
         if (m_ptr)
            return obj.*m_ptr;

         throw std::runtime_error("invalid member pointer");
      }
   };

#if 0
   template <typename T>
   class register_member_setter_and_getter
   {
   public:
      using value_t = T;
   
      using setter_function_t = std::function<value_t&()>;;
      using getter_function_t = std::function<value_t()>;;

   private:
      const std::string_view  m_name{};
      setter_function_t set_f;

   public:
      template <typename Invokable>
      explicit register_member_setter_and_getter(std::string_view name, Invokable f1) noexcept
         : m_name{ name }
         , set_f { std::forward<Invokable>(f1) }
      { }

      auto name() const noexcept { return m_name; }
      
      template <typename U>
      value_t& get(U& obj) const
      {
         throw std::runtime_error("invalid member pointer");
      }

      template <typename U>
      value_t get(U const& obj) const
      {
         throw std::runtime_error("invalid member pointer");
      }
   };
#endif

   //! \brief check if type T is known by the reflection system
   template <typename T>
   struct is_registered : std::integral_constant<bool, !std::is_same_v<std::tuple<>, decltype(metainfo<T>())>> 
   {};

   //! \brief helper variable template 
   template <typename T>
   inline constexpr bool is_registered_v = is_registered<T>::value;


   template <typename Class, typename F>
   void for_each_member(F&& func)
   {
      static_assert(is_registered_v<Class>, "Class has to be known by the reflection system");

      std::apply([&func](auto &&... e) { (..., func(e)); }, metainfo<Class>());
   }
}
