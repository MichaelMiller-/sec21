#pragma once

#include <tuple>
#include <string_view>
#include <stdexcept>
#include <type_traits>
#include <functional>

namespace sec21::reflection
{
   //! \brief every class has to spezialize metainfo<> to introduce T to the reflection system
   template <typename T>
   struct metainfo;

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
   template <typename T, typename Class>
   class register_member_setter_and_getter
   {
   public:
      using value_t = T;
   
      using setter_function_t = std::function<value_t&(Class& obj)>;
      using getter_function_t = std::function<value_t(Class const& obj)>;

   private:
      const std::string_view  m_name{};
      setter_function_t set_f;

   public:
      template <typename Invokable>
      explicit register_member_setter_and_getter(std::string_view name, Invokable&& f1) noexcept
         : m_name{ name }
         , set_f (std::forward<Invokable>(f1)) 
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

   //! \todo move th extra header
   template<template<typename...> class, typename, typename=void>
   struct is_specialized : std::false_type {};

   template<template<typename...> class Template, typename T>
   struct is_specialized<Template, T, std::void_t<decltype(Template<T>{})>> : std::true_type {};

   //! \brief check if type T is known by the reflection system
   template <typename T>
   struct is_registered : is_specialized<metainfo, T> {};

   template <typename T>
   concept Reflected = is_registered<T>::value;

   template <Reflected Class, typename Func>
   void for_each_member(Func&& func)
   {
      std::apply(
         // [&](auto &&... e) { (..., wrapper<decltype(e), Func>{ func }(e)); }, 
         [&](auto &&... e) { (..., func(e)); }, 
         metainfo<Class>::type_info());
   }
}

/*
#include <tuple>
#include <string_view>
#include <stdexcept>
#include <string>
#include <iostream>
#include <iomanip>
#include <functional>
#include <boost/core/demangle.hpp>
#include <iostream>
#include <optional>
template <typename T>
inline auto name() -> std::string
{
    return boost::core::demangle(typeid(T).name());
}
template <typename T>
inline void print_type(std::string n)
{
    std::cout << n << ": " << name<T>() << std::endl;
}


template <auto N>
auto& operator << (std::ostream& os, std::array<bool,N> const& obj) {
    return os; // << "(" << std::get<0>(obj) << "," << std::get<1>(obj) << ")";
}


namespace Wt::Dbo
{
    template <typename T>
    struct ptr {};

    template <class A, typename V>
    void field(A& action, V& value, const std::string& name, int size) {}

    template <class A, class C>
    void field(A& action, ptr<C>& value, const std::string& name, int) {}
}

namespace ns
{
template <typename T>
struct metainfo;

template <typename Class, typename T>
class register_member
{
public:
    using member_ptr_t = T Class::*;
    using value_t = T;

private:
    const std::string_view  m_name{};
    member_ptr_t            m_ptr{ nullptr };

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

    template <typename Function>
    decltype(auto) invoke(Function && func, Class && obj) 
    {
        if (!m_ptr)
            throw std::runtime_error("invalid member pointer");

        return std::invoke(func, obj.*m_ptr);
    }

    auto bar()
    {
        // print_type<Class>("obj");
        std::cout << "member::bar\n";
    }
};

template <typename Class, typename T>
class register_array
{
public:
    using member_ptr_t = T Class::*;
    using class_t = Class;
    using value_t = T;

private:
    const std::string_view  m_name{};
    member_ptr_t            m_ptr{ nullptr };

public:
    explicit register_array(std::string_view name, member_ptr_t ptr) noexcept
        : m_name{ name }
        , m_ptr{ ptr }
    { }

    auto name() const noexcept { return m_name; }
    
    // auto& get(Class& obj) const
    // { 
    //     if (m_ptr)
    //         return obj.*m_ptr;

    //     throw std::runtime_error("invalid member pointer");
    // }

    auto get(Class const& obj) const
    {
        if (m_ptr)
            return obj.*m_ptr;

        throw std::runtime_error("invalid member pointer");
    }
};

template<template<typename...> class, typename, typename=void>
struct is_specialized : std::false_type {};

template<template<typename...> class Template, typename T>
struct is_specialized<Template, T, std::void_t<decltype(Template<T>{})>> : std::true_type {};

template <typename T>
struct is_registered : is_specialized<metainfo, T> {};

template <typename...>
struct is_register_array : std::false_type {};

template <typename Class, typename T>
struct is_register_array<register_array<Class, T>> :  std::true_type {};

template <typename...>
struct is_register_member : std::false_type {};

template <typename Class, typename T>
struct is_register_member<register_member<Class, T>> :  std::true_type {};

static_assert(is_register_member<register_member<int,int>>::value);
static_assert(is_register_member<int>::value == false);
static_assert(is_register_member<double>::value == false);
static_assert(is_register_member<std::string>::value == false);

template <typename T>
concept Reflected = is_registered<T>::value;

template <typename Class, typename T>
struct wrapper
{
    template <typename Func, typename U>
    void operator()(Func && func, U && u)
    {
        print_type<U>("u");
        std::cout << u.name() << std::endl;
        if constexpr (is_register_member<std::decay_t<T>>::value)
        {
            std::cout << "is register member" << std::endl;
            u.bar();
            // u.invoke(func, u);
            // std::invoke(func, std::forward<U>(u));
        }
        if constexpr (is_register_array<std::decay_t<T>>::value)
        {
            std::cout << "is register register_array" << std::endl;
            std::cout << u.name() << ".x" << std::endl;
            std::cout << u.name() << ".y" << std::endl;
            std::cout << u.name() << ".z" << std::endl;
        }
        // return std::invoke(func, std::forward<Args>(args)...);
    }
};

template <Reflected Class, typename Func>
void for_each_member(Func&& func)
{
    std::apply(
        [&](auto &&... e) { (..., wrapper<Class, decltype(e)>{}(func, e)); }, 
        // [&](auto &&... e) { (..., func(e)); }, 
        metainfo<Class>::type_info());
}

///
/// USER CODE
///
struct user
{
   std::string name{};
   std::string password{};
   int         karma{};
};

template <auto Dimension> //, typename Precision = double>
using support = std::array<bool, Dimension>;

template <typename T>
struct descriptor_traits;

template <>
struct descriptor_traits<int>
{
    using type_t = int;

    static constexpr auto invalid() noexcept -> type_t
    {
        return std::numeric_limits<type_t>::max();
    }
};

template <auto Dimension, typename Descriptor = int, typename Precision = double>
struct node
{
    static_assert(Dimension == 2, "Only works in 2D just yet");
    static_assert(std::is_floating_point<Precision>::value, "Precision is not a floating-point type");
    static_assert(std::numeric_limits<Precision>::is_iec559, "Floating-point type does not fulfill the requirements of IEC 559");

    constexpr static auto dimension_v = Dimension;

    using descriptor_t = Descriptor;
    using precision_t = Precision;

    using point_t = std::array<precision_t, dimension_v>;
    using global_support_t = support<dimension_v>;

    //! \brief unique name
    descriptor_t name{ descriptor_traits<descriptor_t>::invalid() };
    //! \brief World Position
    point_t position{};

    std::optional<global_support_t> global_support{};
};

template <auto Dimension, typename Descriptor, typename Precision>
struct metainfo<node<Dimension, Descriptor, Precision>>
{
    using type_t = node<Dimension, Descriptor, Precision>;

    static auto type_info() noexcept
    {
        return std::tuple{ 
            register_member{ "name", &type_t::name },
            register_array{ "position", &type_t::position },
            // register_member{ "position", &type_t::position },
            // register_member{ "global_support", &type_t::global_support },
        };
    }
};


template <typename T>
auto& operator << (std::ostream& os, metainfo<T> const& info) 
{
    for_each_member<metainfo<T>::type_t>([&](auto const& member) { });
    return os; 
}

template <auto Dimension, typename Descriptor, typename Precision>
auto& operator << (std::ostream& os, node<Dimension, Descriptor, Precision> const& n) 
{
    os << "node {\n";
    for_each_member<std::decay_t<decltype(n)>>([&](auto const& member) 
    {
        auto m = member.get(n);
        os << " N: " << std::quoted(member.name());
        os << " T: " << std::quoted(name<decltype(m)>());
        os << " V: "; // << m;
        os << std::endl;
    });
    return os << "}"; 
}

using node_t = node<2, int, double>;
static_assert(is_register_member<register_member<node_t,double>>::value);
static_assert(is_registered<node_t>::value);
static_assert(is_registered<int>::value == false);

template <typename T>
struct dbo_wrapper : T
{
    template <typename ... Args>
    explicit dbo_wrapper(Args &&... args) noexcept : T{ std::forward<Args>(args)... } {} 

    template <typename Action>
    void persist(Action& a) noexcept
    {
        for_each_member<std::decay_t<T>>([&](auto const& member) 
        {
            // using vt = std::decay_t<decltype(member)>::value_t;
            // print_type<vt>("vt");
            auto m = member.get(*this);
        });
    }
};

}

#define REFLECT() \
    friend struct reflect::DefaultResolver; \
    static reflect::TypeDescriptor_Struct Reflection; \
    static void initReflection(reflect::TypeDescriptor_Struct*);

int main()
{
    std::array<double, 2> a2{ { 3.14, 4.4 } };
    std::cout << a2 << std::endl;

    using namespace ns;
    node_t n1{ 55, { 3.14, 4.4 } };

    for_each_member<decltype(n1)>([&](auto const& member) 
    {
        // using vt = std::decay_t<decltype(member)>::value_t;
        // print_type<vt>("vt");
        // auto m = member.get(n1);
        // std::cout << "N: " << std::quoted(member.name()) 
        //     << " T: " << std::quoted(name<decltype(m)>()) 
        //     << " V: " << m << std::endl;
    });

    // std::cout << n1 << std::endl;
    // auto db = dbo_wrapper<node_t>{ std::move(n1) };
    // int z{7};
    // db.persist(z);
}
*/

/*
#include <boost/hana.hpp>
namespace hana = boost::hana;

struct Person 
{
  BOOST_HANA_DEFINE_STRUCT(Person,
    (std::string, name),
    (int, age)
  );
};

auto serialize = [](std::ostream& os, auto const& object) {
  hana::for_each(hana::members(object), [&](auto member) {
    os << member << std::endl;
  });
};

*/